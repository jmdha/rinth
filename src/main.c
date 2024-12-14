#include "io.h"
#include "log.h"
#include "domain.h"
#include "problem.h"
#include "task.h"

int main(int, char **argv) {
    log_init();
    const char* path_domain = argv[1];
    const char* path_problem = argv[2];
    char** fbuf_domain = f_open(path_domain);
    char** fbuf_problem = f_open(path_problem);
    const Domain domain = domain_parse(*fbuf_domain);
    const Problem problem = problem_parse(*fbuf_problem);
    const struct task task = translate(&domain, &problem);
    f_close(fbuf_domain);
    f_close(fbuf_problem);
    return 0;
}
