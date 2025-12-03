#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "parse.h"
#include "log.h"

// TODO: Parses (:predicates (conn ? ?)) as if conn has one parameter

enum kind { 
	EOI,
	LPAREN,
	RPAREN,
	DASH,
	ID,
	MAX_KIND
};

enum keyword {
	KEYWORD_DEFINE,
	KEYWORD_NAME,
	KEYWORD_DOMAIN,
	KEYWORD_REQUIREMENTS,
	KEYWORD_TYPES,
	KEYWORD_PREDICATES,
	KEYWORD_ACTION,
	KEYWORD_PARAMETERS,
	KEYWORD_PRECONDITION,
	KEYWORD_EFFECT,
	KEYWORD_AND,
	KEYWORD_OR,
	KEYWORD_NOT,
	KEYWORD_OBJECTS,
	KEYWORD_INIT,
	KEYWORD_GOAL,
	MAX_KEYWORD
};

const char *KIND_NAMES[MAX_KIND + 1] = {
	"EOI",
	"LPAREN",
	"RPAREN",
	"ID",
	"UNDEFINED"
};

const char *KEYWORD_NAMES[MAX_KEYWORD + 1] = {
	"KEYWORD_DEFINE",
	"KEYWORD_NAME",
	"KEYWORD_DOMAIN",
	"KEYWORD_REQUIREMENTS",
	"KEYWORD_TYPES",
	"KEYWORD_PREDICATES",
	"KEYWORD_ACTION",
	"KEYWORD_PARAMETERS",
	"KEYWORD_PRECONDITION",
	"KEYWORD_EFFECT",
	"KEYWORD_AND",
	"KEYWORD_OR",
	"KEYWORD_NOT",
	"KEYWORD_OBJECTS",
	"KEYWORD_INIT",
	"KEYWORD_GOAL",
	"UNDEFINED"
};

typedef struct {
	const char *str;
	uint        pos;
} lexer;

enum keyword keyword_match(
	const string *str
) {
	if (str->len == 6 && strncmp("define", str->ptr, str->len) == 0)
		return KEYWORD_DEFINE;
	else if (str->len == 6 && strncmp("domain", str->ptr, str->len) == 0)
		return KEYWORD_NAME;
	else if (str->len == 7 && strncmp("problem", str->ptr, str->len) == 0)
		return KEYWORD_NAME;
	else if (str->len == 3 && strncmp("and", str->ptr, str->len) == 0)
		return KEYWORD_AND;
	else if (str->len == 2 && strncmp("or", str->ptr, str->len) == 0)
		return KEYWORD_OR;
	else if (str->len == 3 && strncmp("not", str->ptr, str->len) == 0)
		return KEYWORD_NOT;
	else if (str->len == 13 && strncmp(":requirements", str->ptr, str->len) == 0)
		return KEYWORD_REQUIREMENTS;
	else if (str->len == 6 && strncmp(":types", str->ptr, str->len) == 0)
		return KEYWORD_TYPES;
	else if (str->len == 11 && strncmp(":predicates", str->ptr, str->len) == 0)
		return KEYWORD_PREDICATES;
	else if (str->len == 7 && strncmp(":action", str->ptr, str->len) == 0)
		return KEYWORD_ACTION;
	else if (str->len == 11 && strncmp(":parameters", str->ptr, str->len) == 0)
		return KEYWORD_PARAMETERS;
	else if (str->len == 13 && strncmp(":precondition", str->ptr, str->len) == 0)
		return KEYWORD_PRECONDITION;
	else if (str->len == 7 && strncmp(":effect", str->ptr, str->len) == 0)
		return KEYWORD_EFFECT;
	else if (str->len == 7 && strncmp(":domain", str->ptr, str->len) == 0)
		return KEYWORD_DOMAIN;
	else if (str->len == 8 && strncmp(":objects", str->ptr, str->len) == 0)
		return KEYWORD_OBJECTS;
	else if (str->len == 5 && strncmp(":init", str->ptr, str->len) == 0)
		return KEYWORD_INIT;
	else if (str->len == 5 && strncmp(":goal", str->ptr, str->len) == 0)
		return KEYWORD_GOAL;
	return MAX_KEYWORD;
}

static bool is_whitespace(
	char c
) {
	return ((unsigned char)c) - 1 <= 32;
}

static bool is_text(
	char c
) {
	return c > 41;
}

static void skip_whitespace(
	lexer* l
) {
	while (is_whitespace(l->str[l->pos]))
		l->pos++;
}

static void skip_text(
	lexer* l
) {
	while (is_text(l->str[l->pos]))
		l->pos++;
}

static void next_line(
	lexer* l
) {
	while (l->str[++l->pos] != '\n') {}
}

enum kind lexer_next(
	lexer* l,
	string *str
) {
	skip_whitespace(l);
	const uint pos = l->pos;
	switch (l->str[l->pos++]) {
	case '\0': return EOI;
	case '(':  return LPAREN;
	case ')':  return RPAREN;
	case '-':  return DASH;
	case ';':
		next_line(l);
		return lexer_next(l, str);
	default:
		skip_text(l);
		str->ptr = &l->str[pos];
		str->len = l->pos - pos;
		return ID;
	}
}

void lexer_expect(
	lexer* l,
	enum kind kind
) {
	enum kind actual;
	string tmp;
	if ((actual = lexer_next(l, &tmp)) != kind) {
		fprintf(stderr, "%s: Unexpected token kind\n", KIND_NAMES[kind]);
		exit(1);
	}
}

void lexer_expect_def(
	lexer* l,
	enum keyword kw
) {
	enum kind kind;
	string tmp;
	if ((kind = lexer_next(l, &tmp)) != ID) {
		fprintf(stderr, "%s: Unexpected token kind\n", KIND_NAMES[kind]);
		exit(1);
	}
	enum keyword actual;
	if ((actual = keyword_match(&tmp)) != kw) {
		fprintf(stderr, "%s: Unexpected keyword\n", KEYWORD_NAMES[kind]);
		exit(1);
	}
}

static void parse_id(
	lexer* l,
	string *id
) {
	lexer_next(l, id);
	lexer_expect(l, RPAREN);
}

static void parse_ids(
	lexer* l,
	string *ids,
	uint *count
) {
	*count = 0;
	while (lexer_next(l, &ids[*count]) == ID)
		(*count)++;
}

static void parse_argged(
	lexer* l,
	string* id,
	string* args,
	uint* count
) {
	lexer_next(l, id);
	parse_ids(l, args, count);
}

static void parse_typed(
	lexer* l,
	uint* count,
	string* names,
	string* types,
	bool save
) {
	*count = 0;
	enum kind kind;
	bool parent  = false;
	uint p_index = UINT_MAX;
	while ((kind = lexer_next(l, &names[*count])) != RPAREN) {
		if (kind == ID) {
			if (parent) {
				for (uint i = p_index; i < *count; i++)
					types[i] = names[*count];
				p_index = UINT_MAX;
			}
			if ((save || !parent) && !str_contains(&names[*count], names, *count)) {
				types[*count].ptr = NULL;
				types[*count].len = 0u;
				(*count)++;
			}
			if (!parent && p_index >= *count)
				p_index = *count - 1;
			parent = false;
		} else if (kind == DASH)
			parent = true;
	}
}

static void parse_predicates(
	lexer* l,
	struct predicate* predicates,
	uint* count
) {
	*count = 0;
	while(lexer_next(l, NULL) == LPAREN) {
		struct predicate* predicate = &predicates[(*count)++];
		lexer_next(l, &predicate->name);
		parse_typed(l, &predicate->var_count, predicate->vars, predicate->var_types, false);
	}
}

static void parse_expression(
	lexer* l,
	struct expression** exp
) {
	string tmp;
	if (lexer_next(l, &tmp) == RPAREN) return;
	*exp = malloc(sizeof(struct expression));
	switch (keyword_match(&tmp)) {
	case KEYWORD_OR:
		(*exp)->kind = E_OR;
		(*exp)->nary.count = 0;
		while (lexer_next(l, &tmp) == LPAREN)
		    parse_expression(l, &(*exp)->nary.exps[(*exp)->nary.count++]);
		break;
	case KEYWORD_AND:
		(*exp)->kind = E_AND;
		(*exp)->nary.count = 0;
		while (lexer_next(l, &tmp) == LPAREN)
		    parse_expression(l, &(*exp)->nary.exps[(*exp)->nary.count++]);
		break;
	case KEYWORD_NOT:
		(*exp)->kind = E_NOT;
		lexer_expect(l, LPAREN);
		parse_expression(l, &(*exp)->unary);
		lexer_expect(l, RPAREN);
		break;
	default:
		(*exp)->kind = E_ATOM;
		(*exp)->atom.predicate = tmp;
		parse_ids(l, (*exp)->atom.args, &(*exp)->atom.arg_count);
		break;
	}
}

static void parse_action(
	lexer* l,
	struct action* action
) {
	action->precondition = NULL;
	action->effect       = NULL;
	action->var_count    = 0;
	lexer_next(l, &action->name);
	string tmp = {0};
	while ((lexer_next(l, &tmp)) != RPAREN) {
		enum keyword keyword = keyword_match(&tmp);
		TRACE("Parse %s of action %.*s", KEYWORD_NAMES[keyword], action->name.len, action->name.ptr);
		lexer_expect(l, LPAREN);
		switch (keyword) {
		case KEYWORD_PARAMETERS:   
			parse_typed(l, &action->var_count, action->vars, action->var_types, false); break;
		case KEYWORD_PRECONDITION: parse_expression(l, &action->precondition); break;
		case KEYWORD_EFFECT:       parse_expression(l, &action->effect); break;
		default:
			fprintf(stderr, "%s: Unexpected keyword\n", KEYWORD_NAMES[keyword]);
			exit(1);
		}
	}
}

void parse_domain_(
	struct domain* d,
	const char* str
) {
	lexer l = { .str = str, .pos = 0 };
	d->name.ptr          = NULL;
	d->requirement_count = 0;
	d->predicate_count   = 0;
	d->action_count      = 0;
	lexer_expect(&l, LPAREN);
	lexer_expect_def(&l, KEYWORD_DEFINE);
	string tmp = {0};
	enum kind kind;
	while ((kind = lexer_next(&l, &tmp)) != EOI) {
		if (kind == RPAREN) break;
		assert(kind == LPAREN);
		lexer_next(&l, &tmp);
		enum keyword keyword = keyword_match(&tmp);
		TRACE("Parse %s", KEYWORD_NAMES[keyword]);
		switch (keyword) {
		case KEYWORD_NAME:         parse_id(&l, &d->name); break;
		case KEYWORD_REQUIREMENTS: parse_ids(&l, d->requirements, &d->requirement_count); break;
		case KEYWORD_TYPES:        parse_typed(&l, &d->type_count, d->types, d->type_parents, true); break;
		case KEYWORD_PREDICATES:   parse_predicates(&l, d->predicates, &d->predicate_count); break;
		case KEYWORD_ACTION:       parse_action(&l, &d->actions[d->action_count++]); break;
		default:
			fprintf(stderr, "%s: Unexpected keyword\n", KEYWORD_NAMES[keyword]);
			exit(1);
		}
	}
	INFO("Domain:     %.*s", d->name.len, d->name.ptr);
	INFO("Types:      %d", d->type_count);
	INFO("Predicates: %d", d->predicate_count);
	INFO("Actions:    %d", d->action_count);
}

static void parse_facts(
	lexer* l,
	struct fact*
	facts,
	uint* count
) {
	*count = 0;
	while(lexer_next(l, NULL) == LPAREN) {
		struct fact* fact = &facts[(*count)++];
		parse_argged(l, &fact->predicate, fact->args, &fact->arg_count);
	}
}

static void parse_goal(
	lexer* l,
	struct fact* facts,
	uint* count
) {
	lexer_expect(l, LPAREN);
	lexer_expect_def(l, KEYWORD_AND);
	parse_facts(l, facts, count);
	lexer_expect(l, RPAREN);
}

void parse_problem_(
	struct problem* p,
	const char* str
) {
	lexer l = { .str = str, .pos = 0 };
	p->name.ptr     = NULL;
	p->domain.ptr   = NULL;
	p->object_count = 0;
	p->init_count   = 0;
	p->goal_count   = 0;
	lexer_expect(&l, LPAREN);
	lexer_expect_def(&l, KEYWORD_DEFINE);
	string tmp = {0};
	enum kind kind;
	while ((kind = lexer_next(&l, &tmp)) != EOI) {
		if (kind == RPAREN) break;
		assert(kind == LPAREN);
		lexer_next(&l, &tmp);
		enum keyword keyword = keyword_match(&tmp);
		TRACE("Parse %s", KEYWORD_NAMES[keyword]);
		switch (keyword) {
		case KEYWORD_NAME:    parse_id(&l, &p->name); break;
		case KEYWORD_DOMAIN:  parse_id(&l, &p->domain); break;
		case KEYWORD_OBJECTS: parse_typed(&l, &p->object_count, p->objects, p->object_types, false); break;
		case KEYWORD_INIT:    parse_facts(&l, p->inits, &p->init_count); break;
		case KEYWORD_GOAL:    parse_goal(&l, p->goals, &p->goal_count); break;
		default:
			fprintf(stderr, "%s: Unexpected keyword\n", KEYWORD_NAMES[keyword]);
			exit(1);
		}
	}
	INFO("Problem: %.*s", p->name.len, p->name.ptr);
	INFO("Objects: %d", p->object_count);
	INFO("Inits:   %d", p->init_count);
	INFO("Goals:   %d", p->goal_count);
}

struct domain parse_domain(
	const char* str
) {
	struct domain domain;
	parse_domain_(&domain, str);
	return domain;
}

struct problem parse_problem(
	const char* str
) {
	struct problem problem;
	parse_problem_(&problem, str);
	return problem;
}

static void expression_free(
	struct expression* exp
) {
	switch (exp->kind) {
		case E_ATOM: break;
		case E_NOT:
			expression_free(exp->unary);
			break;
		case E_AND:
		case E_OR:
			for (uint i = 0; i < exp->nary.count; i++)
				expression_free(exp->nary.exps[i]);
			break;
	}
	free(exp);
}

void domain_free(
	struct domain* domain
) {
	for (uint i = 0; i < domain->action_count; i++) {
		if (domain->actions[i].precondition)
			expression_free(domain->actions[i].precondition);
		if (domain->actions[i].effect)
			expression_free(domain->actions[i].effect);
	}
}
