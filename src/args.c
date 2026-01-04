#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "args.h"

void print_help(void) {

}

args args_parse(int argc, char** argv) {
	args a = { 0 };

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
				printf("unknown search algorithm\n");
				exit(1);
			}
			break;
		case 'e':
			if (strcmp("cp", optarg) == 0)
				a.expand = EXPAND_CP;
			else if (strcmp("sqlite", optarg) == 0)
				a.expand = EXPAND_SQLITE;
			else {
				printf("unknown expansion strategy\n");
				exit(1);
			}
			break;
		case 'h':
			if (strcmp("goal_count", optarg) == 0)
				a.eval = EVAL_GOAL_COUNT;
			else {
				printf("unknown heuristic\n");
				exit(1);
			}
			break;
		default:
			print_help();
		}
	}

	return a;
}

