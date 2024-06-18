#include <memory.h>
#include <stdlib.h>

#include "domain.h"
#include "lexer.h"
#include "log.h"

static inline void
ParseList(char **list, int *count, TokenKind kind, Token *t, Lexer *lexer, const char *str) {
    *count = 0;
    while (LexerNext(t, lexer)) {
        if (t->kind == kind)
            WriteToken(&list[(*count)++], t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, kind, RPAREN);
    }
    EOI(RPAREN);
}

static inline void ParseName(char **name, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteToken(name, t, str);
    ExpectNext(t, lexer, RPAREN);
}

static inline void
ParseRequirements(char **list, int *count, Token *t, Lexer *lexer, const char *str) {
    while (LexerNext(t, lexer)) {
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

static inline void ParsePredicate(Predicate *predicate, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteToken(&predicate->name, t, str);
    ParseList(predicate->vars, &predicate->var_count, VARIABLE, t, lexer, str);
}

static inline void
ParsePredicates(Predicate *list, int *count, Token *t, Lexer *lexer, const char *str) {
    while (LexerNext(t, lexer)) {
        if (t->kind == LPAREN)
            ParsePredicate(&(list[(*count)++]), t, lexer, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, LPAREN, RPAREN);
    }
    EOI(RPAREN);
}

static inline void ParseExpression(Expression **exp, Token *t, Lexer *lexer, const char *str) {
    if (!LexerNext(t, lexer)) EOI(RPAREN);
    if (t->kind == RPAREN) return;
    Expression *e = malloc(sizeof(Expression));
    *exp          = e;
    switch (t->kind) {
    case ID:
        e->kind = E_ATOM;
        WriteToken(&e->data.atom.predicate, t, str);
        ParseList(e->data.atom.vars, &e->data.atom.var_count, VARIABLE, t, lexer, str);
        break;
    case EXP_NOT:
        e->kind = E_NOT;
        ExpectNext(t, lexer, LPAREN);
        ParseExpression(&e->data.unary, t, lexer, str);
        ExpectNext(t, lexer, RPAREN);
        break;
    case EXP_OR: e->kind = E_OR; goto C_NARY;
    case EXP_AND:
        e->kind = E_AND;
    C_NARY:
        e->data.nary.count = 0;
        while (LexerNext(t, lexer)) {
            if (t->kind == RPAREN) return;
            if (t->kind != LPAREN) Expected("LPAREN", t->kind);
            ParseExpression(&e->data.nary.exps[e->data.nary.count++], t, lexer, str);
        }
        ExpectNext(t, lexer, RPAREN);
        break;
    default: free(e); Expected("EXPRESSION or RPAREN", t->kind);
    };
}

static inline void ParseAction(Action *action, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteToken(&action->name, t, str);
    TRACE("Parsing action %s", action->name);
    ExpectNext(t, lexer, DEF_PARAMETERS);
    TRACE("Parsing parameters of %s", action->name);
    ExpectNext(t, lexer, LPAREN);
    ParseList(action->vars, &action->var_count, VARIABLE, t, lexer, str);
    ExpectNext(t, lexer, DEF_PRECONDITION);
    TRACE("Parsing precondition of %s", action->name);
    ExpectNext(t, lexer, LPAREN);
    ParseExpression(&action->precondition, t, lexer, str);
    ExpectNext(t, lexer, DEF_EFFECT);
    TRACE("Parsing effect of %s", action->name);
    ExpectNext(t, lexer, LPAREN);
    ParseExpression(&action->effect, t, lexer, str);
    ExpectNext(t, lexer, RPAREN);
}

Domain DomainParse(const char *str) {
    Domain domain = {.name = NULL, .predicate_count = 0, .action_count = 0};
    Lexer lexer   = {.str = str, .pos = 0};
    Token t;

    ExpectNext(&t, &lexer, LPAREN);
    ExpectNext(&t, &lexer, DEF_DEFINE);
    while (LexerNext(&t, &lexer)) {
        if (t.kind == RPAREN) break;
        Expect(t.kind, LPAREN);
        LexerNext(&t, &lexer);
        TRACE("Parsing %s token in domain parsing", TOKEN_NAMES[t.kind]);
        switch (t.kind) {
        case DEF_NAME: ParseName(&domain.name, &t, &lexer, str); break;
        case DEF_REQUIREMENTS:
            ParseRequirements(domain.requirements, &domain.requirement_count, &t, &lexer, str);
            break;
        case DEF_PREDICATES:
            ParsePredicates(domain.predicates, &domain.predicate_count, &t, &lexer, str);
            break;
        case DEF_ACTION:
            ParseAction(&domain.actions[domain.action_count++], &t, &lexer, str);
            break;
        default: ERROR("Unexpected token %s", TOKEN_NAMES[t.kind]); exit(1);
        }
    }

    DEBUG("Predicates: %d", domain.predicate_count);
    DEBUG("Actions: %d", domain.action_count);

    if (domain.predicate_count > MAX_PREDICATES * 0.5)
        WARN("Domain has %d predicates, which is near maximum.", domain.predicate_count);
    if (domain.action_count > MAX_ACTIONS * 0.5)
        WARN("Domain has %d actions, which is near maximum.", domain.action_count);

    return domain;
}

static inline void ExpressionDelete(Expression *exp) {
    switch (exp->kind) {
    case E_ATOM:
        free(exp->data.atom.predicate);
        for (int i = 0; i < exp->data.atom.var_count; i++)
            free(exp->data.atom.vars[i]);
        break;
    case E_NOT: ExpressionDelete(exp->data.unary); break;
    case E_AND:
    case E_OR:
        for (int i = 0; i < exp->data.nary.count; i++)
            ExpressionDelete(exp->data.nary.exps[i]);
        break;
    }
    free(exp);
}

void DomainDelete(Domain *domain) {
    free(domain->name);
    for (int i = 0; i < domain->requirement_count; i++)
        free(domain->requirements[i]);
    for (int i = 0; i < domain->predicate_count; i++) {
        Predicate *p = &domain->predicates[i];
        free(p->name);
        for (int t = 0; t < p->var_count; t++)
            free(p->vars[t]);
    }
    for (int i = 0; i < domain->action_count; i++) {
        Action *a = &domain->actions[i];
        free(a->name);
        for (int t = 0; t < a->var_count; t++)
            free(a->vars[t]);
        ExpressionDelete(a->precondition);
        ExpressionDelete(a->effect);
    }
}
