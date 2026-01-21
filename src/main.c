#include <stdio.h>

#include "expand.h"
#include "io.h"
#include "log.h"
#include "pddl.h"
#include "task.h"
#include "translate.h"
#include "search.h"
#include "eval.h"

int main(int argc, char** argv) {
        log_init();

	const args   a            = args_parse(argc, argv);
        char**       domain_str   = f_open(a.path_domain);
        char**       problem_str  = f_open(a.path_problem);
        pddl_domain  domain       = pddl_domain_parse(*domain_str);
        pddl_problem problem      = pddl_problem_parse(*problem_str);
        task         def          = translate_pddl(&domain, &problem);

        INFO("Domain:     %.*s", domain.name.len, domain.name.ptr);
        INFO("Problem:    %.*s", problem.name.len, problem.name.ptr);
	INFO("Predicates: %zu", slen(def.predicates));
	INFO("Objects:    %zu", slen(def.objects));

        expand_init(&def, a.expand);
	eval_init(&def, a.eval);

        INFO("INITIALIZATION FINISHED");

        path sol = solve(def.init, def.goal, a.search);
	printf("%zu\n", sol.len);
	
	task_free(&def);
	f_close(domain_str);
	f_close(problem_str);

        return 0;
}
