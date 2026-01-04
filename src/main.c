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

	const args   a            = args_parse(argc, argv);
        char**       domain_str   = f_open(a.path_domain);
        char**       problem_str  = f_open(a.path_problem);
        pddl_domain  domain       = pddl_domain_parse(*domain_str);
        pddl_problem problem      = pddl_problem_parse(*problem_str);
        task         def          = translate_pddl(&domain, &problem);
        INFO("Domain:  %.*s", domain.name.len, domain.name.ptr);
        INFO("Problem: %.*s", problem.name.len, problem.name.ptr);

	task_print(&def);

        expand_init(&def, a.expand);

        INFO("INITIALIZATION FINISHED");

        path sol = solve(def.init, def.goal, a.search);
	printf("%zu\n", sol.len);
	//for (size_t i = 0; i < sol.len; i++) {
	//	const action* a = &def.actions[sol.actions[i]];
	//	printf("(%.*s", (int) a->name.len, a->name.ptr);
	//	for (size_t t = 0; t < a->arity; t++)
	//		printf(" %.*s", (int) def.objects[sol.args[i][t]].len, def.objects[sol.args[i][t]].ptr);
	//	printf(")\n");
	//}
	
	task_free(&def);
	f_close(domain_str);
	f_close(problem_str);

        return 0;
}
