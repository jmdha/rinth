#include "task.h"
#include "log.h"

static void convert_facts(struct fact *facts, const SFact *sfacts, const uint len) {
    
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
    convert_facts(task.inits, problem->inits, problem->init_count);
    TRACE("Translating goal");
    convert_facts(task.goals, problem->goals, problem->goal_count);

    return task;
}
