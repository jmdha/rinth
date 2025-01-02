#include "eval.h"
#include "expand.h"
#include "io.h"
#include "log.h"
#include "parse.h"
#include "search.h"
#include "state.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>

int main(int, char **argv) {
    log_init();
    srand(0);
    const char* path_domain = argv[1];
    const char* path_problem = argv[2];
    char** fbuf_domain = f_open(path_domain);
    char** fbuf_problem = f_open(path_problem);
    struct domain domain = parse_domain(*fbuf_domain);
    struct problem problem = parse_problem(*fbuf_problem);
    struct task task = translate(&domain, &problem);
    state* init = state_clone(task.init);
    state* goal = state_clone(task.goal);
    expand_init(&task);
    eval_init(&task);
    domain_free(&domain);
    task_free(&task);
    INFO("INITIALIZATION FINISHED");
    printf("Solve: %d\n", solve(NULL, init, goal));
    return 0;
}
