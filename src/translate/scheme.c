#include <stb_ds.h>

#include "log.h"
#include "scheme.h"

static Atom TranslateAtom(char **v_predicates, char **vars, uint count, Expression *expression, bool val) {
    int predicate = -1;
    for (uint i = 0; i < arrlenu(v_predicates); i++)
        if (strcmp(v_predicates[i], expression->data.atom.predicate) == 0) predicate = i;
    if (predicate == -1) ERROR("Action has undeclared predicate %s", expression->data.atom.predicate);

    uint *v_args = NULL;
    for (uint i = 0; i < expression->data.atom.var_count; i++) {
        int arg = -1;
        for (uint t = 0; t < count; t++)
            if (strcmp(vars[t], expression->data.atom.vars[i]) == 0) arg = t;
        if (predicate == -1) {
            ERROR("Action has undeclared arg %s", expression->data.atom.vars[i]);
            abort();
        }
        arrpush(v_args, arg);
    }

    Atom atom = {.predicate = predicate, .v_args = v_args, .val = val};
    return atom;
}

static Atom *TranslateExpression(char **v_predicates, char **vars, uint count, Expression *expression) {
    typedef struct {
        Expression *expression;
        bool val;
    } Element;
    Atom *v_atoms    = NULL;
    Element *v_queue = NULL;

    Element init = {.expression = expression, .val = true};
    arrpush(v_queue, init);

    while (arrlenu(v_queue) > 0) {
        Element e = arrpop(v_queue);
        switch (e.expression->kind) {
        case E_ATOM: arrpush(v_atoms, TranslateAtom(v_predicates, vars, count, e.expression, e.val)); break;
        case E_AND:
            for (uint i = 0; i < e.expression->data.nary.count; i++) {
                Element child = {.expression = e.expression->data.nary.exps[i], .val = e.val};
                arrpush(v_queue, child);
            }
            break;
        case E_NOT:
            e.val        = !e.val;
            e.expression = e.expression->data.unary;
            arrpush(v_queue, e);
            break;
        case E_OR: break;
        }
    }

    arrfree(v_queue);
    return v_atoms;
}

Scheme TranslateAction(char **v_predicates, Action *action) {
    TRACE("Translating action %s", action->name);
    Scheme scheme = {
        .name  = action->name,
        .v_pre = TranslateExpression(v_predicates, action->vars, action->var_count, action->precondition),
        .v_eff = TranslateExpression(v_predicates, action->vars, action->var_count, action->effect),
    };
    action->name = NULL;
    return scheme;
}

Scheme *TranslateActions(char **v_predicates, Action *actions, uint count) {
    Scheme *v_schemes = NULL;

    for (uint i = 0; i < count; i++)
        arrpush(v_schemes, TranslateAction(v_predicates, &actions[i]));

    return v_schemes;
}
