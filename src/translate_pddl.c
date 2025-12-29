#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "translate.h"

void translate_predicates(string* out, const pddl_pred* in) {
        while ((in++)->name.ptr)
                memcpy(out++, &(in++)->name, sizeof(string));
}

void translate_objects(string* out, const string* in) {
        while (in->ptr)
                memcpy(out++, in++, sizeof(string));
}

// Assumes flat expression
void translate_expression(atom* pos, atom* neg, const string* exp, const string* vars,
                          const string* preds) {
        size_t nots[100];
        size_t count = 0;
        size_t depth = 0;
        while (exp->ptr) {
                // for (size_t i = 0; i < depth; i++)
                //	printf("\t");
                // printf("%.*s\n", exp->len, exp->ptr);
                if (exp->ptr[0] == '(') {
                        depth++;
                } else if (exp->ptr[0] == ')') {
                        depth--;
                } else if (strncmp(exp->ptr, "not", exp->len)) {
                        count++;
                } else if (strncmp(exp->ptr, "and", exp->len)) {
                } else {
                        if (count % 2 == 0) {

                                pos++;
                        } else {

                                neg++;
                        }
                }
                exp++;
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
        while (in->name.ptr)
                translate_action(out++, in++, preds);
}

void translate_fact(atom* out, const pddl_atom* in, const string* preds, const string* objects) {
        out->predicate = sfind(preds, &in->predicate) - preds;
        out->arity     = slen(in->vars);
}

void translate_facts(atom* out, const pddl_atom* in, const string* preds, const string* objects) {
        while (in->predicate.ptr)
                translate_fact(out++, in++, preds, objects);
}

task translate_pddl(const pddl_domain* d, const pddl_problem* p) {
        task t = {0};

        TRACE("Validate");
        pddl_validate(d, p);

        TRACE("Translate");
        translate_predicates(t.predicates, d->predicates);
        translate_objects(t.objects, p->objects);
        translate_actions(t.actions, d->actions, t.predicates);
        translate_facts(t.inits, p->inits, t.predicates, t.objects);
        translate_facts(t.goals, p->goals, t.predicates, t.objects);

        return t;
}
