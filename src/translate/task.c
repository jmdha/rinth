#include "task.h"
#include "misc.h"
#include <string.h>

static void TranslateInit(uint *count, Fact *facts, const Domain *domain, const Problem *problem) {
    *count = problem->init_count;
    //for (uint i = 0; i < *count; i++) {
    //    const SFact *fact = &problem->inits[i];
    //    facts[i].predicate = IndexOf(domain->predicates, domain->predicate_count, &fact->predicate, sizeof(string), StringCmp);
    //}
}

Task Translate(const Domain *domain, const Problem *problem) {
    Task task            = {0};
    task.predicate_count = domain->predicate_count;
    memcpy(task.predicates, domain->predicates, domain->predicate_count * sizeof(string));
    task.object_count = problem->object_count;
    memcpy(task.objects, problem->objects, problem->object_count * sizeof(string));
    TranslateInit(&task.init_count, task.inits, domain, problem);
    return task;
}
