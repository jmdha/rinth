#pragma once

#include "misc.h"
#include "parse/problem.h"
#include "translate/scheme.h"

typedef struct {
    uint predicate;
    uint *v_args;
} Fact;

bool FactEqual(const Fact* a, const Fact* b);
Fact *TranslateFacts(char **v_predicates, char **v_objects, uint count, SFact *init);
uint *IndexizeFacts(Fact *v_facts, Fact *v_to_be_indexed);
