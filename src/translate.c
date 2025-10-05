#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "translate.h"
#include "log.h"

u16 predicate_index(
    const string*           str,
    const struct predicate* predicates,
    const uint              count
) {
    uint pred_index = UINT_MAX;
    for (uint t = 0; t < count; t++) {
        if (str_cmp(str, &predicates[t].name)) {
            pred_index = t;
            break;
        }
    }
    if (pred_index == UINT_MAX) {
        fprintf(stderr, "%.*s: Undefined predicate referenced\n", str->len, str->ptr);
        exit(1);
    }
    return pred_index;
}

static void convert_types_to_facts(
    state*        s,
    const uint    type_count,
    const string* type_parents,
    const string* types,
    const uint    object_count,
    const string* object_types
) {
    u16 arg;
    for (uint i = 0; i < object_count; i++) {
        const string* type = &object_types[i];
        while (type->ptr) {
            const uint type_index = str_index(type, types, type_count);
            arg = i;
            state_insert(s, type_index, 1, &arg);
            type = &type_parents[type_index];
        }
    }
}

static void convert_facts(
    state*                  s,
    const struct fact*      facts,
    const uint              len,
    const struct predicate* predicates,
    const uint              predicate_count,
    const string*           objects,
    const uint              object_count
) {
    u16 args[16];
    for (uint i = 0; i < len; i++) {
        const u16 p = predicate_index(&facts[i].predicate, predicates, predicate_count);
        for (uint t = 0; t < facts[i].arg_count; t++)
            args[t] = str_index(&facts[i].args[t], objects, object_count);
        state_insert(s, p, facts[i].arg_count, args);
    }
}

// Converts an expressions into multiple expressions which consist only of a conjuction of atoms
// I.e. Remove or which might lead to multiple expressions
static uint flatten_expression(
    struct expression*       exps,
    const struct expression* exp
) {
    memcpy(&exps[0], exp, sizeof(struct expression));
    return 1;
}


typedef struct {
    const struct expression* exp;
    bool                     val;
} ExpVal;

// Converts an expression which may only be a conjunction of atoms (or nots)
// Should generally call flatten_expression first
static uint convert_expression(
    Atom*                    atoms,
    const struct expression* exp,
    const string*            vars,
    const uint               var_count,
    const struct predicate*  predicates,
    const uint               predicate_count
) {
    uint count = 0;
    ExpVal queue[256];
    uint   queue_len  = 0;
    assert(exp->kind == E_AND);
    for (uint i = 0; i < exp->nary.count; i++) {
        queue[queue_len].exp = exp->nary.exps[i]; 
        queue[queue_len].val = true;
        queue_len++;
    }
    while (queue_len) {
        ExpVal e_val = queue[--queue_len];
        switch (e_val.exp->kind) {
            case E_ATOM:
                atoms[count].val       = e_val.val;
                atoms[count].predicate = predicate_index(
                    &e_val.exp->atom.predicate, 
                    predicates, predicate_count
                );
                atoms[count].arg_count = e_val.exp->atom.arg_count;
                for (uint i = 0; i < e_val.exp->atom.arg_count; i++)
                    atoms[count].args[i] = str_index(&e_val.exp->atom.args[i], vars, var_count);
                count++;
                break;
            case E_NOT:
                queue[queue_len].exp = e_val.exp->unary;
                queue[queue_len].val = !e_val.val;
                queue_len++;
                break;
            case E_AND:
            case E_OR:
                fprintf(stderr, "Internal error: convert_expression called with non flat expression\n");
                exit(1);
        }
    }
    return count;
}

// One action can lead to multiple schemes. 
// This occurs in the case of OR expressions (only possible in precondition)
// Each sub-expression will lead to a differing scheme, and even more in the case of multiple OR's
static void convert_action(
    Scheme*                 schemes,
    uint*                   count,
    const struct action*    action,
    const struct predicate* predicates,
    const uint              predicate_count
) {
    struct expression pre_exps[MAX_SCHEMES];
    struct expression eff_exps[MAX_SCHEMES];
    uint pre_exp_count = flatten_expression(pre_exps, action->precondition);
    uint eff_exp_count = flatten_expression(eff_exps, action->effect);
    for (uint i = 0; i < pre_exp_count; i++) {
        Atom pre_atoms[MAX_ATOMS];
        uint pre_atom_count = convert_expression(
            pre_atoms, &pre_exps[i], 
            action->vars, action->var_count, 
            predicates, predicate_count
        );
        for (uint t = 0; t < eff_exp_count; t++) {
            Atom eff_atoms[MAX_ATOMS];
            uint eff_atom_count = convert_expression(
                eff_atoms, &eff_exps[t],
                action->vars, action->var_count,
                predicates, predicate_count
            );

            Scheme *scheme    = &schemes[(*count)++];
            scheme->name      = action->name;
            scheme->vars      = action->var_count;
            scheme->pre_count = pre_atom_count;
            scheme->eff_count = eff_atom_count;
            memcpy(scheme->pre, pre_atoms, pre_atom_count * sizeof(Atom));
            memcpy(scheme->eff, eff_atoms, eff_atom_count * sizeof(Atom));
        }
    }
    INFO(
        "Action %.*s translated to %d schemes",
        action->name.len, action->name.ptr, pre_exp_count * eff_exp_count
    );
}

struct task translate(const struct domain* domain, const struct problem* problem) {
    struct task task = {0};
    uint offset;

    task.domain_name  = domain->name;
    task.problem_name = problem->name;

    TRACE("Translate types");
    task.predicate_count = 0;
    for (uint i = 0; i < domain->type_count; i++) {
        task.predicates[i]     = domain->types[i];
        task.predicate_vars[i] = 1;
        task.predicate_count++;
    }

    TRACE("Translate predicates");
    offset = task.predicate_count;
    for (uint i = 0; i < domain->predicate_count; i++) {
        task.predicates[offset + i]     = domain->predicates[i].name;
        task.predicate_vars[offset + i] = domain->predicates[i].var_count;
        task.predicate_count++;
    }
    INFO("Predicates: %d", task.predicate_count);

    TRACE("Translate objects");
    task.object_count   = 0;
    for (uint i = 0; i < problem->object_count; i++) {
        task.objects[i] = problem->objects[i];
        task.object_count++;
    }
    INFO("Objects: %d", task.object_count);

    task.init = state_new();
    task.goal = state_new();

    TRACE("Translate type facts");
    convert_types_to_facts(
        task.init,
        domain->type_count, domain->type_parents, domain->types,
        problem->object_count, problem->object_types
    );
    TRACE("Translate init");
    convert_facts(
        task.init, problem->inits, problem->init_count,
        domain->predicates, domain->predicate_count,
        problem->objects, problem->object_count
    );
    TRACE("Translate goal");
    convert_facts(
        task.goal, problem->goals, problem->goal_count,
        domain->predicates, domain->predicate_count,
        problem->objects, problem->object_count
    );
    TRACE("Translate actions");
    for (uint i = 0; i < domain->action_count; i++) {
        TRACE("Translate action %.*s", domain->actions[i].name.len, domain->actions[i].name.ptr);
        convert_action(
            task.schemes, &task.scheme_count, 
            &domain->actions[i], 
            domain->predicates, domain->predicate_count
        );
    }
    INFO("Facts init: %d", state_count(task.init));
    INFO("Facts goal: %d", state_count(task.goal));
    INFO("Schemes:    %d", task.scheme_count);
    return task;

}

