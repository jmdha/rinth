#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "task.h"
#include "log.h"

static void convert_fact(
    struct fact*     fact,
    const SFact*    sfact,
    const Predicate *predicates,
    const uint       predicate_count,
    const string    *objects,
    const uint       object_count
) {
    // Find index of predicate
    uint pred_index = UINT_MAX;
    for (uint t = 0; t < predicate_count; t++) {
        if (str_cmp(&sfact->predicate, &predicates[t].name)) {
            pred_index = t;
            break;
        }
    }
    if (pred_index == UINT_MAX) {
        fprintf(
            stderr, 
            "%.*s: Fact refers to undefined predicate\n", sfact->predicate.len, sfact->predicate.ptr
        );
        exit(1);
    }
    fact->predicate = pred_index;
    // Find indexes of arguments
    for (uint t = 0; t < sfact->arg_count; t++) {
        const string* object = &sfact->args[t];
        uint obj_index = UINT_MAX;
        for (uint p = 0; p < object_count; p++) {
            if (str_cmp(object, &objects[p])) {
                obj_index = p;
                break;
            }
        }
        if (obj_index == UINT_MAX) {
            fprintf(
                stderr, 
                "%.*s: Fact refers to undefined object\n", object->len, object->ptr
            );
            exit(1);
        }
    }
}

static void convert_facts(
    struct fact*     facts,
    const SFact*     sfacts,
    const uint       len,
    const Predicate *predicates,
    const uint       predicate_count,
    const string    *objects,
    const uint       object_count
) {
    for (uint i = 0; i < len; i++)
        convert_fact(&facts[i], &sfacts[i], predicates, predicate_count, objects, object_count);
}

struct task translate(const Domain* domain, const Problem* problem) {
    struct task task = {0};

    task.domain_name  = domain->name;
    task.problem_name = problem->name;

    task.predicate_count = domain->predicate_count;
    for (uint i = 0; i < domain->predicate_count; i++) {
        task.predicate_vars[i] = domain->predicates[i].var_count;
        task.predicates[i]     = domain->predicates[i].name;
    }

    task.object_count = problem->object_count;
    for (uint i = 0; i < problem->object_count; i++)
        task.objects[i] = problem->objects[i];

    task.init_count = problem->init_count;
    task.goal_count = problem->goal_count;

    TRACE("Translating init");
    convert_facts(
        task.inits, problem->inits, problem->init_count,
        domain->predicates, domain->predicate_count,
        problem->objects, problem->object_count
    );
    TRACE("Translating goal");
    convert_facts(
        task.goals, problem->goals, problem->goal_count,
        domain->predicates, domain->predicate_count,
        problem->objects, problem->object_count
    );

    return task;
}
