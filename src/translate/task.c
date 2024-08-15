#include <stb_ds.h>

#include "task.h"

static char **LoadPredicates(uint count, Predicate *predicates) {
    char **v_predicates = NULL;

    for (uint i = 0; i < count; i++) {
        arrpush(v_predicates, predicates[i].name);
        predicates[i].name = NULL;
    }

    return v_predicates;
}

static char **LoadObjects(uint count, char **objects) {
    char **v_objects = NULL;

    for (uint i = 0; i < count; i++) {
        arrpush(v_objects, objects[i]);
        v_objects[i] = NULL;
    }

    return v_objects;
}

Task Translate(Domain *domain, Problem *problem) {
    char **v_predicates = LoadPredicates(domain->predicate_count, domain->predicates);
    char **v_objects    = LoadObjects(problem->object_count, problem->objects);
    Task task           = {.v_predicates = v_predicates, .v_objects = v_objects};
    return task;
}

void TaskDelete(Task *) {}
