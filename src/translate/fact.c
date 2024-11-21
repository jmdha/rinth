#include <stb_ds.h>
#include <stddef.h>

#include "fact.h"
#include "log.h"

bool FactEqual(const Fact *a, const Fact *b) {
    if (a->predicate != b->predicate) return false;
    for (uint i = 0; i < arrlenu(a->v_args); i++)
        if (a->v_args[i] != b->v_args[i]) return false;
    return true;
}

static Fact TranslateFact(char **v_predicates, char **v_objects, SFact *init) {
//    int predicate = -1;
//    for (uint i = 0; i < arrlenu(v_predicates); i++)
//        if (strcmp(v_predicates[i], init->predicate) == 0) predicate = i;
//    if (predicate == -1) ERROR("Undeclared predicate %s in init", init->predicate);
//    uint *v_args = NULL;
//    for (uint i = 0; i < init->arg_count; i++) {
//        int arg = -1;
//        for (uint t = 0; t < arrlenu(v_objects); t++)
//            if (strcmp(v_objects[t], init->args[i]) == 0) arg = t;
//        if (arg == -1) ERROR("Undeclared object %s in init", init->args[i]);
//        arrpush(v_args, arg);
//    }
//    Fact fact = {.predicate = predicate, .v_args = v_args};
//    return fact;
}

Fact *TranslateFacts(char **v_predicates, char **v_objects, uint count, SFact *init) {
    Fact *v_facts = NULL;

    for (uint i = 0; i < count; i++)
        arrpush(v_facts, TranslateFact(v_predicates, v_objects, &init[i]));

    return v_facts;
}

uint *IndexizeFacts(Fact *v_facts, Fact *v_to_be_indexed) {
    uint *v_indexes = NULL;

    for (uint i = 0; i < arrlenu(v_to_be_indexed); i++) {
        int fact_index = -1;
        for (uint t = 0; t < arrlenu(v_facts); t++) {
            if (v_facts->predicate != v_to_be_indexed->predicate) continue;
            bool equal = true;
            for (uint arg = 0; arg < arrlenu(v_to_be_indexed->v_args); arg++)
                if (v_facts->v_args[arg] != v_to_be_indexed->v_args[arg]) equal = false;
            if (equal) fact_index = t;
        }
        if (fact_index == -1) ERROR("Internal error. Trying to index unknown fact.");
        arrpush(v_indexes, fact_index);
    }

    return v_indexes;
}

void FactFree(Fact *fact) {
    arrfree(fact->v_args);
}
