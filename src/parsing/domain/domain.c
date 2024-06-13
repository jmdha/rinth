#include "domain.h"
#include "parsing/domain/lexer.h"
#include "parsing/domain/token.h"
#include <stdlib.h>

int DomainParse(const char *str, Domain *domain) {
    Lexer lexer = LexerInit();
    Token token = LexerNext(&lexer, str);
    while (token.kind != TOKEN_EOF) {
        token = LexerNext(&lexer, str);
    }
    return 1;
}

void DomainDelete(Domain *domain) {
    free(domain->name);
    for (int i = 0; i < domain->predicate_count; i++)
        PredicateDelete(&domain->predicates[i]);
    for (int i = 0; i < domain->action_count; i++)
        ActionDelete(&domain->actions[i]);
}

void ActionDelete(Action *action) {
    free(action->name);
    for (int i = 0; i < action->parameter_count; i++)
        ParameterDelete(&action->parameters[i]);
    ExpressionDelete(action->precondition);
    ExpressionDelete(action->effect);
    free(action);
}

void ExpressionDelete(Expression *expression) {
    switch (expression->kind) {
    case EQUAL:
    case AND:
    case OR: {
        for (int i = 0; i < expression->data.NARY.children_count; i++)
            ExpressionDelete(expression->data.NARY.children[i]);
        break;
    }
    case NOT: ExpressionDelete(expression->data.UNARY.child); break;
    default: break;
    }
    free(expression);
}

void PredicateDelete(Predicate *predicate) {
    free(predicate->name);
    for (int i = 0; i < predicate->parameter_count; i++)
        ParameterDelete(&predicate->parameters[i]);
    free(predicate);
}

void ParameterDelete(Parameter *parameter) {
    free(parameter->name);
    free(parameter);
}
