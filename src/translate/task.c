#include <stb_ds.h>

#include "log.h"
#include "task.h"
#include "translate/fact.h"
#include "translate/scheme.h"

static char **LoadPredicates(uint count, Predicate *predicates) {
    char **v_predicates = NULL;

    for (uint i = 0; i < count; i++) {
        //arrpush(v_predicates, predicates[i].name);
        //predicates[i].name = NULL;
    }

    return v_predicates;
}

static char **LoadObjects(uint count, char **objects) {
    char **v_objects = NULL;

    for (uint i = 0; i < count; i++) {
        arrpush(v_objects, objects[i]);
        objects[i] = NULL;
    }

    return v_objects;
}

Task Translate(Domain *domain, Problem *problem) {
    TRACE("Translating predicates");
    char **v_predicates = LoadPredicates(domain->predicate_count, domain->predicates);
    TRACE("Translating objects");
    char **v_objects = LoadObjects(problem->object_count, problem->objects);
    TRACE("Translating actions");
    Scheme *v_schemes = TranslateActions(v_predicates, domain->actions, domain->action_count);
    TRACE("Translating init");
    Fact *v_init_facts = TranslateFacts(v_predicates, v_objects, problem->init_count, problem->inits);
    TRACE("Translating goal");
    Fact *v_goal_facts = TranslateFacts(v_predicates, v_objects, problem->goal_count, problem->goals);
    TRACE("Generating facts");
    //Fact *v_facts = GenerateFacts(v_schemes, v_init_facts);
    //TRACE("Indexing init");
    //uint *v_init = IndexizeFacts(v_facts, v_init_facts);
    //TRACE("Indexing goal");
    //uint *v_goal = IndexizeFacts(v_facts, v_goal_facts);
    
    for (uint i = 0; i < arrlenu(v_init_facts); i++)
        FactFree(&v_init_facts[i]);
    arrfree(v_init_facts);
    for (uint i = 0; i < arrlenu(v_goal_facts); i++)
        FactFree(&v_goal_facts[i]);
    arrfree(v_goal_facts);
    for (uint i = 0; i < arrlenu(v_schemes); i++)
        SchemeFree(&v_schemes[i]);
    arrfree(v_schemes);
    Task task    = {
           .v_predicates = v_predicates, //
           .v_objects    = v_objects,
           //.v_facts      = v_facts,
           //.v_init       = v_init,
           //.v_goal       = v_goal
    };
    return task;
}

void TaskDelete(Task *task) {
    for (uint i = 0; i < arrlenu(task->v_predicates); i++)
        free(task->v_predicates[i]);
    arrfree(task->v_predicates);
    for (uint i = 0; i < arrlenu(task->v_objects); i++)
        free(task->v_objects[i]);
    arrfree(task->v_objects);
    for (uint i = 0; i < arrlenu(task->v_operators); i++)
        OperatorFree(&task->v_operators[i]);
    arrfree(task->v_operators);
    for (uint i = 0; i < arrlenu(task->v_facts); i++)
        FactFree(&task->v_facts[i]);
    arrfree(task->v_facts);
    arrfree(task->v_init);
    arrfree(task->v_goal);
}
