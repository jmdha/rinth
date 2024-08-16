#include <memory.h>
#include <stdlib.h>

#include "domain.h"
#include "lexer.h"
#include "log.h"

static void ParseList(char **list, uint *count, TokenKind kind, Token *t, const char *str) {
    *count = 0;
    while (LexerNext(t)) {
        if (t->kind == kind)
            WriteToken(&list[(*count)++], t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, kind, RPAREN);
    }
    EOI(RPAREN);
}

static void ParseName(char **name, Token *t, const char *str) {
    ExpectNext(t, ID);
    WriteToken(name, t, str);
    ExpectNext(t, RPAREN);
}

static void ParseRequirements(char **list, uint *count, Token *t, const char *str) {
    while (LexerNext(t)) {
        if (t->kind == RPAREN) return;
        bool is_requirement;
        switch (t->kind) {
        case REQ_STRIPS: is_requirement = true; break;
        default: is_requirement = false; break;
        }
        if (!is_requirement) Expected("REQUIREMENT or RPAREN", t->kind);
        WriteToken(&list[(*count)++], t, str);
    }
    EOI(RPAREN);
}

static void ParsePredicate(Predicate *predicate, Token *t, const char *str) {
    ExpectNext(t, ID);
    WriteToken(&predicate->name, t, str);
    ParseList(predicate->vars, &predicate->var_count, VARIABLE, t, str);
}

static void ParsePredicates(Predicate *list, uint *count, Token *t, const char *str) {
    while (LexerNext(t)) {
        if (t->kind == LPAREN)
            ParsePredicate(&(list[(*count)++]), t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, LPAREN, RPAREN);
    }
    EOI(RPAREN);
}

static void ParseExpression(Expression **exp, Token *t, const char *str) {
    if (!LexerNext(t)) EOI(RPAREN);
    if (t->kind == RPAREN) return;
    Expression *e = malloc(sizeof(Expression));
    *exp          = e;
    switch (t->kind) {
    case ID:
        e->kind = E_ATOM;
        WriteToken(&e->data.atom.predicate, t, str);
        ParseList(e->data.atom.vars, &e->data.atom.var_count, VARIABLE, t, str);
        break;
    case EXP_NOT:
        e->kind = E_NOT;
        ExpectNext(t, LPAREN);
        ParseExpression(&e->data.unary, t, str);
        ExpectNext(t, RPAREN);
        break;
    case EXP_OR: e->kind = E_OR; goto C_NARY;
    case EXP_AND:
        e->kind = E_AND;
    C_NARY:
        e->data.nary.count = 0;
        while (LexerNext(t)) {
            if (t->kind == RPAREN) return;
            if (t->kind != LPAREN) Expected("LPAREN", t->kind);
            ParseExpression(&e->data.nary.exps[e->data.nary.count++], t, str);
        }
        ExpectNext(t, RPAREN);
        break;
    default: free(e); Expected("EXPRESSION or RPAREN", t->kind);
    };
}

static void ParseAction(Action *action, Token *t, const char *str) {
    ExpectNext(t, ID);
    WriteToken(&action->name, t, str);
    TRACE("Parsing action %s", action->name);
    ExpectNext(t, DEF_PARAMETERS);
    TRACE("Parsing parameters of %s", action->name);
    ExpectNext(t, LPAREN);
    ParseList(action->vars, &action->var_count, VARIABLE, t, str);
    ExpectNext(t, DEF_PRECONDITION);
    TRACE("Parsing precondition of %s", action->name);
    ExpectNext(t, LPAREN);
    ParseExpression(&action->precondition, t, str);
    ExpectNext(t, DEF_EFFECT);
    TRACE("Parsing effect of %s", action->name);
    ExpectNext(t, LPAREN);
    ParseExpression(&action->effect, t, str);
    ExpectNext(t, RPAREN);
}

Domain DomainParse(const char *str) {
    LexerInit(str);
    Domain domain = {.name = NULL, .predicate_count = 0, .action_count = 0};
    Token t;

    ExpectNext(&t, LPAREN);
    ExpectNext(&t, DEF_DEFINE);
    while (LexerNext(&t)) {
        if (t.kind == RPAREN) break;
        Expect(t.kind, LPAREN);
        LexerNext(&t);
        TRACE("Parsing %s token in domain parsing", TOKEN_NAMES[t.kind]);
        switch (t.kind) {
        case DEF_NAME: ParseName(&domain.name, &t, str); break;
        case DEF_REQUIREMENTS:
            ParseRequirements(domain.requirements, &domain.requirement_count, &t, str);
            break;
        case DEF_PREDICATES:
            ParsePredicates(domain.predicates, &domain.predicate_count, &t, str);
            break;
        case DEF_ACTION: ParseAction(&domain.actions[domain.action_count++], &t, str); break;
        default: ERROR("Unexpected token %s", TOKEN_NAMES[t.kind]); exit(1);
        }
    }

    INFO("Predicates: %d", domain.predicate_count);
    INFO("Actions: %d", domain.action_count);

    if (domain.predicate_count > MAX_PREDICATES * 0.5)
        WARN("Domain has %d predicates, which is near maximum.", domain.predicate_count);
    if (domain.action_count > MAX_ACTIONS * 0.5)
        WARN("Domain has %d actions, which is near maximum.", domain.action_count);

    return domain;
}

static void ExpressionDelete(Expression *exp) {
    switch (exp->kind) {
    case E_ATOM:
        free(exp->data.atom.predicate);
        for (uint i = 0; i < exp->data.atom.var_count; i++)
            free(exp->data.atom.vars[i]);
        break;
    case E_NOT: ExpressionDelete(exp->data.unary); break;
    case E_AND:
    case E_OR:
        for (uint i = 0; i < exp->data.nary.count; i++)
            ExpressionDelete(exp->data.nary.exps[i]);
        break;
    }
    free(exp);
}

void DomainDelete(Domain *domain) {
    free(domain->name);
    for (uint i = 0; i < domain->requirement_count; i++)
        free(domain->requirements[i]);
    for (uint i = 0; i < domain->predicate_count; i++) {
        Predicate *p = &domain->predicates[i];
        free(p->name);
        for (uint t = 0; t < p->var_count; t++)
            free(p->vars[t]);
    }
    for (uint i = 0; i < domain->action_count; i++) {
        Action *a = &domain->actions[i];
        free(a->name);
        for (uint t = 0; t < a->var_count; t++)
            free(a->vars[t]);
        ExpressionDelete(a->precondition);
        ExpressionDelete(a->effect);
    }
}
