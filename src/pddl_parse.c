#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pddl.h"
#include "log.h"

typedef enum { EOI, LPAREN, RPAREN, ID, MAX_KIND } kind;

bool is_whitespace(char c) {
        return ((unsigned char)c) - 1 <= 32;
}

bool is_text(char c) {
        return c > 41;
}

const char* skip_whitespace(const char* l) {
        while (is_whitespace(*l))
                l++;
        return l;
}

const char* skip_text(const char* l) {
        while (is_text(*l))
                l++;
        return l;
}

const char* next_line(const char* l) {
        while (*l != '\n')
                l++;
        return l;
}

kind lexer_next(const char** l, string* s) {
        *l = skip_whitespace(*l);

        s->ptr = (*l)++;
        s->len = 1;
        switch (*s->ptr) {
        case '\0':
                return EOI;
        case '(':
                return LPAREN;
        case ')':
                return RPAREN;
        case ';':
                *l = next_line(*l);
                return lexer_next(l, s);
        default:
                *l     = skip_text(*l);
                s->len = *l - s->ptr;
                return ID;
        }
}

void parse_ids(const char** l, string* ids) {
        while (lexer_next(l, ids) == ID)
                ids++;
        ids[0].ptr = NULL;
        ids[0].len = 0;
}

void parse_predicates(const char** l, pddl_pred* preds) {
        string t = {0};
        while (lexer_next(l, &t) == LPAREN) {
                assert(lexer_next(l, &preds[0].name) == ID);
                parse_ids(l, preds[0].vars);
                preds++;
        }
}

void parse_expression(const char** l, string* stack) {
        size_t d = 0;
        do {
                kind k = lexer_next(l, stack++);
                if (k == LPAREN)
                        d++;
                else if (k == RPAREN)
                        d--;
        } while (d > 0);
}

void parse_action(const char** l, pddl_action* action) {
        string t = {0};
        kind   k = 0;

        assert(lexer_next(l, &action->name) == ID);
        while (lexer_next(l, &t) != RPAREN) {
                if (strncmp(t.ptr, ":parameters", t.len) == 0) {
                        assert(lexer_next(l, &t) == LPAREN);
                        parse_ids(l, action->vars);
                } else if (strncmp(t.ptr, ":precondition", t.len) == 0) {
                        parse_expression(l, action->pre);
                } else if (strncmp(t.ptr, ":effect", t.len) == 0) {
                        parse_expression(l, action->eff);
                } else {
                        fprintf(stderr, "unexpected keyword %.*s\n", (int)t.len, t.ptr);
                        exit(1);
                }
        }
}

const char* pddl_domain_match(pddl_domain* d, const char* l) {
        string t = {0};
        kind   k = 0;

        assert(lexer_next(&l, &t) == ID);
        if (strncmp(t.ptr, "domain", t.len) == 0) {
		TRACE("Parse domain name");
                assert(lexer_next(&l, &d->name) == ID);
                assert(lexer_next(&l, &t) == RPAREN);
        } else if (strncmp(t.ptr, ":requirements", t.len) == 0) {
		TRACE("Parse domain requirements");
                parse_ids(&l, d->requirements);
        } else if (strncmp(t.ptr, ":predicates", t.len) == 0) {
		TRACE("Parse domain predicates");
                parse_predicates(&l, d->predicates);
        } else if (strncmp(t.ptr, ":action", t.len) == 0) {
		TRACE("Parse domain action");
                parse_action(&l, &d->actions[0]);
        } else {
                fprintf(stderr, "unexpected keyword %.*s\n", (int)t.len, t.ptr);
                exit(1);
        }

        return l;
}

pddl_domain pddl_domain_parse(const char* s) {
        pddl_domain d = {0};
        string      t = {0};
        kind        k = 0;

	TRACE("Parse domain");

        assert(lexer_next(&s, &t) == LPAREN);
        assert(lexer_next(&s, &t) == ID);

        while ((k = lexer_next(&s, &t)) != EOI) {
                if (k == RPAREN)
                        break;
                assert(k == LPAREN);
                s = pddl_domain_match(&d, s);
        }

        return d;
}

void parse_atoms(const char** l, pddl_atom* atoms) {
        string t = {0};
        while (lexer_next(l, &t) == LPAREN) {
                assert(lexer_next(l, &atoms[0].predicate) == ID);
                parse_ids(l, atoms[0].vars);
                atoms++;
        }
}

const char* pddl_problem_match(pddl_problem* p, const char* l) {
        string t = {0};
        kind   k = 0;

        assert(lexer_next(&l, &t) == ID);
        if (strncmp(t.ptr, "problem", t.len) == 0) {
		TRACE("Parse problem name");
                assert(lexer_next(&l, &p->name) == ID);
                assert(lexer_next(&l, &t) == RPAREN);
	} else if (strncmp(t.ptr, ":domain", t.len) == 0) {
		TRACE("Parse problem domain");
                assert(lexer_next(&l, &p->domain) == ID);
                assert(lexer_next(&l, &t) == RPAREN);
        } else if (strncmp(t.ptr, ":objects", t.len) == 0) {
		TRACE("Parse problem objects");
                parse_ids(&l, p->objects);
	} else if (strncmp(t.ptr, ":init", t.len) == 0) {
		TRACE("Parse problem inits");
		parse_atoms(&l, p->inits);
	} else if (strncmp(t.ptr, ":goal", t.len) == 0) {
		TRACE("Parse problem goals");
                assert(lexer_next(&l, &t) == LPAREN);
                assert(lexer_next(&l, &t) == ID);
		parse_atoms(&l, p->goals);
                assert(lexer_next(&l, &t) == RPAREN);
	}


        return l;
}

pddl_problem pddl_problem_parse(const char* s) {
        pddl_problem p = {0};
        string       t = {0};
        kind         k = 0;

	TRACE("Parse problem");

        assert(lexer_next(&s, &t) == LPAREN);
        assert(lexer_next(&s, &t) == ID);

        while ((k = lexer_next(&s, &t)) != EOI) {
                if (k == RPAREN)
                        break;

                assert(k == LPAREN);
                s = pddl_problem_match(&p, s);
        }

        return p;
}

