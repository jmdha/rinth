#include "expand.h"
#include "io.h"
#include "log.h"
#include "pddl.h"
#include "task.h"
#include "translate.h"

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

        expand_init(&def);
        // eval_init(&def);
        // search_init(&def);

        INFO("INITIALIZATION FINISHED");

        // plan         solution     = solve(def.init, def.goal);

        return 0;
}
