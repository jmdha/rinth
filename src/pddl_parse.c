#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pddl.h"
#include "parse.h"

typedef enum { EOI, LPAREN, RPAREN, ID, MAX_KIND } kind;

typedef struct {
	// Start of string
	const char* str;
	// Current position of lexer
	const char* ptr;
} lexer;

const char* kind_names[MAX_KIND] = {
	"EOI", "LPAREN", "RPAREN", "ID"
};

void unexpected_keyword(const lexer* l, const string* s) {
	char buf[1000];
	sprintf(buf, "unexpected keyword %.*s", (int) s->len, s->ptr);
	parse_error(l->str, l->ptr - l->str - s->len, buf, s->len);
}

kind lexer_next(lexer* l, string* s) {
	skip_whitespace(&l->ptr);
	s->ptr = l->ptr++;
	s->len = 1;
	switch (*s->ptr) {
		case '\0': return EOI;
		case '(':  return LPAREN;
		case ')':  return RPAREN;
		case ';':
			   next_line(&l->ptr);
			   return lexer_next(l, s);
		default:
			   skip_text(&l->ptr);
			   s->len = l->ptr - s->ptr;
			   return ID;

	}
}

void lexer_expect(lexer* l, string* s, kind k) {
	kind o = lexer_next(l, s);
	if (o != k) {
		char buf[1000];
		sprintf(buf, "expected symbol %s found %s", kind_names[k], kind_names[o]);
		parse_error(l->str, l->ptr - l->str - s->len, buf, s->len);
	}
}

void lexer_expect_(lexer* l, kind k) {
	string s;
	kind o = lexer_next(l, &s);
	if (o != k) {
		char buf[1000];
		sprintf(buf, "expected symbol %s found %s", kind_names[k], kind_names[o]);
		parse_error(l->str, l->ptr - l->str - s.len, buf, s.len);
	}
}

kind lexer_expect2(lexer* l, string* s, kind k1, kind k2) {
	kind o = lexer_next(l, s);
	if (o != k1 && o != k2) {
		char buf[1000];
		sprintf(buf, "expected symbol %s or %s found %s", kind_names[k1], kind_names[k2], kind_names[o]);
		parse_error(l->str, l->ptr - l->str - s->len, buf, s->len);
	}
	return o;
}

kind lexer_expect2_(lexer* l, kind k1, kind k2) {
	string s;
	kind o = lexer_next(l, &s);
	if (o != k1 && o != k2) {
		char buf[1000];
		sprintf(buf, "expected symbol %s or %s found %s", kind_names[k1], kind_names[k2], kind_names[o]);
		parse_error(l->str, l->ptr - l->str - s.len, buf, s.len);
	}
	return o;
}

void parse_id(lexer* l, string* s) {
	lexer_expect(l, s, ID);
	lexer_expect_(l, RPAREN);
}

void parse_ids(lexer* l, string* s) {
	while (lexer_expect2(l, s, ID, RPAREN) == ID)
		s++;
	s[0].ptr = NULL;
	s[0].len = 0;
}

void parse_predicates(lexer* l, string* s) {
	string tmp;
	while (lexer_expect2(l, &tmp, LPAREN, RPAREN) == LPAREN) {
		lexer_expect(l, s, ID);
		while (lexer_expect2(l, &tmp, ID, RPAREN) == ID) {}
		s++;
	}
	s[0].ptr = NULL;
	s[0].len = 0;
}

void parse_expression(lexer* l, string* stack) {
        size_t d = 1;
	stack[0].ptr = l->ptr - 1;
	stack[0].len = 1;
	stack++;
        do {
		switch (lexer_next(l, stack++)) {
		case LPAREN: d++; break;
		case RPAREN: d--; break;
		default: break;
		}
        } while (d > 0);
}

void parse_action(lexer* l, pddl_action* a) {
	string tmp;
	lexer_expect(l, &a->name, ID);
	while (lexer_expect2(l, &tmp, ID, RPAREN) == ID) {
		lexer_expect_(l, LPAREN);
		if (strncmp(tmp.ptr, ":parameters", tmp.len) == 0)
			parse_ids(l, a->vars);
		else if (strncmp(tmp.ptr, ":precondition", tmp.len) == 0)
			parse_expression(l, a->pre);
		else if (strncmp(tmp.ptr, ":effect", tmp.len) == 0)
			parse_expression(l, a->eff);
		else
			unexpected_keyword(l, &tmp);
	}
}

void domain_parse_next(lexer* l, pddl_domain* d, size_t* a) {
	string s;

	lexer_expect(l, &s, ID);
	if (strncmp(s.ptr, "domain", s.len) == 0)
		parse_id(l, &d->name);
	else if (strncmp(s.ptr, ":requirements", s.len) == 0)
		parse_ids(l, d->requirements);
	else if (strncmp(s.ptr, ":predicates", s.len) == 0)
		parse_predicates(l, d->predicates);
	else if (strncmp(s.ptr, ":action", s.len) == 0)
		parse_action(l, &d->actions[(*a)++]);
	else
		unexpected_keyword(l, &s);
}

pddl_domain pddl_domain_parse(const char* s) {
        pddl_domain d = { 0 };
	lexer       l = { .str = s, .ptr = s };
	size_t      a = 0;

	lexer_expect_(&l, LPAREN);
	lexer_expect_(&l, ID);

	while (lexer_expect2_(&l, LPAREN, RPAREN) == LPAREN)
		domain_parse_next(&l, &d, &a);

	return d;
}

void parse_atoms(lexer* l, pddl_atom* atoms) {
	while (lexer_expect2_(l, LPAREN, RPAREN) == LPAREN) {
		lexer_expect(l, &atoms[0].predicate, ID);
		parse_ids(l, atoms[0].vars);
		atoms++;
	}
}

void problem_parse_next(lexer* l, pddl_problem* p) {
	string s;

	lexer_expect(l, &s, ID);
	if (strncmp(s.ptr, "problem", s.len) == 0)
		parse_id(l, &p->name);
	else if (strncmp(s.ptr, ":domain", s.len) == 0)
		parse_id(l, &p->domain);
	else if (strncmp(s.ptr, ":objects", s.len) == 0)
		parse_ids(l, p->objects);
	else if (strncmp(s.ptr, ":init", s.len) == 0)
		parse_atoms(l, p->inits);
	else if (strncmp(s.ptr, ":goal", s.len) == 0) {
		lexer_expect_(l, LPAREN);
		lexer_expect(l, &s, ID);
		if (strncmp(s.ptr, "and", s.len) != 0)
			unexpected_keyword(l, &s);
		parse_atoms(l, p->goals);
		lexer_expect_(l, RPAREN);
	} else
		unexpected_keyword(l, &s);
}

pddl_problem pddl_problem_parse(const char* s) {
        pddl_problem p = { 0 };
	lexer        l = { .str = s, .ptr = s };

	lexer_expect_(&l, LPAREN);
	lexer_expect_(&l, ID);

	while (lexer_expect2_(&l, LPAREN, RPAREN) == LPAREN)
		problem_parse_next(&l, &p);

        return p;
}
