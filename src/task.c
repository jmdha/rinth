#include <stdint.h>
#include <stdio.h>

#include "task.h"

task task_init(void) {
        task def = {0};

        for (size_t i = 0; i < MAX_ACTIONS; i++) {
                for (size_t t = 0; t < MAX_ATOMS; t++) {
                        def.actions[i].pre_pos[t].predicate = SIZE_MAX;
                        def.actions[i].pre_pos[t].arity     = SIZE_MAX;
                        for (size_t j = 0; j < MAX_VARS; j++)
                                def.actions[i].pre_pos[t].vars[j] = SIZE_MAX;
                }
                for (size_t t = 0; t < MAX_ATOMS; t++) {
                        def.actions[i].pre_neg[t].predicate = SIZE_MAX;
                        def.actions[i].pre_neg[t].arity     = SIZE_MAX;
                        for (size_t j = 0; j < MAX_VARS; j++)
                                def.actions[i].pre_neg[t].vars[j] = SIZE_MAX;
                }
                for (size_t t = 0; t < MAX_ATOMS; t++) {
                        def.actions[i].eff_pos[t].predicate = SIZE_MAX;
                        def.actions[i].eff_pos[t].arity     = SIZE_MAX;
                        for (size_t j = 0; j < MAX_VARS; j++)
                                def.actions[i].eff_pos[t].vars[j] = SIZE_MAX;
                }
                for (size_t t = 0; t < MAX_ATOMS; t++) {
                        def.actions[i].eff_neg[t].predicate = SIZE_MAX;
                        def.actions[i].eff_neg[t].arity     = SIZE_MAX;
                        for (size_t j = 0; j < MAX_VARS; j++)
                                def.actions[i].eff_neg[t].vars[j] = SIZE_MAX;
                }
        }

        return def;
}

void task_print(const task* def) {
	printf("PREDICATES\n");
	for (const string* s = def->predicates; s->ptr; s++)
		printf("\t%.*s\n", (int) s->len, s->ptr);
	printf("OBJECTS\n");
	for (const string* s = def->objects; s->ptr; s++)
		printf("\t%.*s\n", (int) s->len, s->ptr);
	printf("ACTIONS\n");
	for (const action* a = def->actions; a->name.ptr; a++) {
		printf("\t%.*s\n", (int) a->name.len, a->name.ptr);
		printf("\t\tARITY: %zu\n", a->arity);
		printf("\t\tPRE_POS\n");
		for (const atom* m = a->pre_pos; m->predicate != SIZE_MAX; m++) {
			printf("\t\t\t%.*s", (int) def->predicates[m->predicate].len, def->predicates[m->predicate].ptr);
			for (size_t i = 0; i < m->arity; i++)
				printf(" %zu", m->vars[i]);
			printf("\n");
		}
		printf("\t\tPRE_NEG\n");
		for (const atom* m = a->pre_neg; m->predicate != SIZE_MAX; m++) {
			printf("\t\t\t%.*s", (int) def->predicates[m->predicate].len, def->predicates[m->predicate].ptr);
			for (size_t i = 0; i < m->arity; i++)
				printf(" %zu", m->vars[i]);
			printf("\n");
		}
		printf("\t\tEFF_POS\n");
		for (const atom* m = a->eff_pos; m->predicate != SIZE_MAX; m++) {
			printf("\t\t\t%.*s", (int) def->predicates[m->predicate].len, def->predicates[m->predicate].ptr);
			for (size_t i = 0; i < m->arity; i++)
				printf(" %zu", m->vars[i]);
			printf("\n");
		}
		printf("\t\tEFF_NEG\n");
		for (const atom* m = a->eff_neg; m->predicate != SIZE_MAX; m++) {
			printf("\t\t\t%.*s", (int) def->predicates[m->predicate].len, def->predicates[m->predicate].ptr);
			for (size_t i = 0; i < m->arity; i++)
				printf(" %zu", m->vars[i]);
			printf("\n");
		}
	}
}
