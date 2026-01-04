#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "args.h"

void print_help(void) {

}

args args_parse(int argc, char** argv) {
	args a = { 0 };

	a.search = SEARCH_BFS;
	a.expand = EXPAND_CP;
	a.eval   = EVAL_GOAL_COUNT;

	int opt;
	while ((opt = getopt(argc, argv, "t:d:p:s:e:h:")) != -1) {
		switch (opt) {
		case 't':
			a.path_task = optarg;
			break;
		case 'd':
			a.path_domain = optarg;
			break;
		case 'p':
			a.path_problem = optarg;
			break;
		case 's':
			if (strcmp("bfs", optarg) == 0)
				a.search = SEARCH_BFS;
			else if (strcmp("dfs", optarg) == 0)
				a.search = SEARCH_DFS;
			else if (strcmp("gbfs", optarg) == 0)
				a.search = SEARCH_GBFS;
			else if (strcmp("lazy", optarg) == 0)
				a.search = SEARCH_LAZY;
			else {
				fprintf(stderr, "%s: unknown search algorithm\n", optarg);
				exit(1);
			}
			break;
		case 'e':
			if (strcmp("cp", optarg) == 0)
				a.expand = EXPAND_CP;
			else if (strcmp("sqlite", optarg) == 0)
				a.expand = EXPAND_SQLITE;
			else {
				fprintf(stderr, "%s: unknown expansion policy\n", optarg);
				exit(1);
			}
			break;
		case 'h':
			if (strcmp("goal_count", optarg) == 0)
				a.eval = EVAL_GOAL_COUNT;
			else {
				fprintf(stderr, "%s: unknown heuristic\n", optarg);
				exit(1);
			}
			break;
		default:
			print_help();
		}
	}

	return a;
}

