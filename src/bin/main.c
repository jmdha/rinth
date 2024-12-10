#include "io.h"
#include "log.h"
#include "parse/domain.h"
#include "parse/problem.h"
#include "translate/task.h"

int init(
    char **path_domain,
    char **path_problem,
    int argc,
    char **argv
) {
    log_init();
    INFO("Reading args");
    if (argc < 3) {
        return 1;
    }
    *path_domain  = argv[1];
    *path_problem = argv[2];
    return 0;
}

int parse(
    Domain*     domain,
    Problem*    problem,
    const char* path_domain,
    const char* path_problem
) {
    char** fbuf_domain  = f_open(path_domain);
    char** fbuf_problem = f_open(path_problem);
    DomainParse(domain, *fbuf_domain);
    ProblemParse(problem, *fbuf_problem);
    f_close(fbuf_domain);
    f_close(fbuf_problem);
    return 0;
}

int main(int argc, char **argv) {
    int status;
    char* path_domain;
    char* path_problem;
    if ((status = init(&path_domain, &path_problem, argc, argv))) {
        return status;
    }
    Domain  domain;
    Problem problem;
    if ((status = parse(&domain, &problem, path_domain, path_problem))) {
        return status;
    }
    struct task task;
    if ((status = translate(&task, &domain, &problem))) {
        return status;
    }
    return 0;
}
