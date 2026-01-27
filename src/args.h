#ifndef ARGS_H
#define ARGS_H

typedef enum {
	EXPAND_NONE,
	EXPAND_CP,
	EXPAND_SQLITE,
} expand_kind;

typedef enum {
	SEARCH_NONE,
	SEARCH_BFS,
	SEARCH_GBFS,
	SEARCH_LAZY,
	SEARCH_BEAM,
} search_kind;

typedef enum {
	EVAL_NONE,
	EVAL_GOAL_COUNT,
} eval_kind;

typedef struct {
	char*       path_task;
	char*       path_domain;
	char*       path_problem;
	expand_kind expand;
	search_kind search;
	eval_kind   eval;
} args;

args args_parse(int argc, char** argv);

#endif
