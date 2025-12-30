#include <stdio.h>

#include "expand.h"
#include "io.h"
#include "log.h"
#include "pddl.h"
#include "task.h"
#include "translate.h"
#include "search.h"

int main(int argc, char** argv) {
        log_init();

        const char*  domain_path  = argv[1];
        const char*  problem_path = argv[2];
        char**       domain_str   = f_open(domain_path);
        char**       problem_str  = f_open(problem_path);
        pddl_domain  domain       = pddl_domain_parse(*domain_str);
        pddl_problem problem      = pddl_problem_parse(*problem_str);
        task         def          = translate_pddl(&domain, &problem);
        INFO("Domain:  %.*s", domain.name.len, domain.name.ptr);
        INFO("Problem: %.*s", problem.name.len, problem.name.ptr);

	task_print(&def);

        expand_init(&def);

        INFO("INITIALIZATION FINISHED");

        path sol = solve(def.init, def.goal);
	printf("%zu\n", sol.len);
	//for (size_t i = 0; i < sol.len; i++) {
	//	const action* a = &def.actions[sol.actions[i]];
	//	printf("(%.*s", (int) a->name.len, a->name.ptr);
	//	for (size_t t = 0; t < a->arity; t++)
	//		printf(" %.*s", (int) def.objects[sol.args[i][t]].len, def.objects[sol.args[i][t]].ptr);
	//	printf(")\n");
	//}

        return 0;
}
