#include "io.h"
#include "log.h"
#include "parse/domain.h"
#include "parse/problem.h"
#include "translate/task.h"
#include "path.h"

int init(
    char **path_domain,
    char **path_problem,
    int argc,
    char **argv
) {
    log_init();
    INFO("Reading args");
    if (argc < 3) {
        ERROR("Requires at least 2 args: domain path and problem path.");
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

int solve(
    struct path*       path,
    const struct task* task
) {
    return 1;
}

int main(int argc, char **argv) {
    int status;
    char* path_domain;
    char* path_problem;
    if ((status = init(&path_domain, &path_problem, argc, argv))) {
        ERROR("Failed init");
        return status;
    }
    Domain  domain;
    Problem problem;
    if ((status = parse(&domain, &problem, path_domain, path_problem))) {
        ERROR("Failed parse");
        return status;
    }
    struct task task;
    if ((status = translate(&task, &domain, &problem))) {
        ERROR("Failed translate");
        return status;
    }
    struct path path;
    if ((status = solve(&path, &task))) {
        ERROR("Failed solve");
        return status;
    }   
    return 0;
}
