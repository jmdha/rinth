#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "translate.h"

void translate_predicates(string* out, const pddl_pred* in) {
        TRACE("Translate predicates");
        while ((in)->name.ptr)
                memcpy(out++, &(in++)->name, sizeof(string));
}

void translate_objects(string* out, const string* in) {
        TRACE("Translate objects");
        while (in->ptr)
                memcpy(out++, in++, sizeof(string));
}

// Assumes flat expression
// HACK: This function stinks (maybe generate expression tree before this?)
void translate_expression(atom* pos, atom* neg, const string* exp, const string* vars,
                          const string* preds) {
        TRACE("Translate expression");
        size_t nots[1000];
        size_t count  = 0;
        size_t depth  = 0;
        size_t e_atom = SIZE_MAX;
        for (size_t i = 0; exp[i].ptr; i++) {
                if (exp[i].ptr[0] == '(') {
                        depth++;
                } else if (exp[i].ptr[0] == ')') {
                        if (e_atom != SIZE_MAX) {
                                atom* a      = count % 2 == 0 ? pos++ : neg++;
                                a->predicate = sfind(preds, &exp[e_atom]) - preds;
                                for (size_t t = e_atom + 1; t < i; t++)
                                        a->vars[t - e_atom - 1] = sfind(vars, &exp[t]) - vars;
                                a->arity = i - e_atom - 1;
                                e_atom   = SIZE_MAX;
                        }

                        if (count != 0 && nots[count - 1] == depth)
                                count--;
                        depth--;
                } else if (3 == exp[i].len && strncmp(exp[i].ptr, "not", exp[i].len) == 0) {
                        nots[count++] = depth;
                } else if (3 == exp[i].len && strncmp(exp[i].ptr, "and", exp[i].len) == 0) {
                } else {
                        if (e_atom == SIZE_MAX) {
                                e_atom = i;
                                continue;
                        }
                }
        }
}

void translate_action(action* out, const pddl_action* in, const string* preds) {
        TRACE("Translate action %.*s", in->name.len, in->name.ptr);

        out->name  = in->name;
        out->arity = slen(in->vars);

        translate_expression(out->pre_pos, out->pre_neg, in->pre, in->vars, preds);
        translate_expression(out->eff_pos, out->eff_neg, in->eff, in->vars, preds);
}

void translate_actions(action* out, const pddl_action* in, const string* preds) {
        TRACE("Translate actions");
        while (in->name.ptr)
                translate_action(out++, in++, preds);
}

atom translate_fact(const pddl_atom* in, const string* preds, const string* objects) {
	atom out = {0};
        out.predicate = sfind(preds, &in->predicate) - preds;
        out.arity     = slen(in->vars);
        for (size_t i = 0; in->vars[i].ptr; i++)
                out.vars[i] = sfind(objects, &in->vars[i]) - objects;
	return out;
}

void translate_facts(state** out, const pddl_atom* in, const string* preds, const string* objects) {
        TRACE("Translate facts");
	*out = state_new();
        while (in->predicate.ptr) {
                const atom a = translate_fact(in++, preds, objects);
		state_insert(*out, a.predicate, a.arity, a.vars);
	}
}

task translate_pddl(const pddl_domain* d, const pddl_problem* p) {
        task t = task_init();

        TRACE("Validate");
        pddl_validate(d, p);

        TRACE("Translate");
        translate_predicates(t.predicates, d->predicates);
        translate_objects(t.objects, p->objects);
        translate_actions(t.actions, d->actions, t.predicates);
        translate_facts(&t.init, p->inits, t.predicates, t.objects);
        translate_facts(&t.goal, p->goals, t.predicates, t.objects);

        return t;
}
