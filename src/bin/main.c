#include "io.h"
#include "log.h"
#include "parse/domain.h"
#include "parse/problem.h"
#include "parse/verify.h"
#include "translate/task.h"

int main(int argc, char **argv) {
    LogInit();
    INFO("Reading args");
    if (argc < 3) {
        ERROR("Requires at least 2 args: domain path and problem path.");
        return 1;
    }
    const char *domain_path  = argv[1];
    const char *problem_path = argv[2];
    INFO("Opening domain file");
    fbuf fb_domain = f_open(domain_path);
    INFO("Parsing domain");
    Domain domain;
    DomainParse(&domain, *fb_domain);
    INFO("Opening problem file");
    fbuf fb_problem = f_open(problem_path);
    INFO("Parsing problem");
    Problem problem;
    ProblemParse(&problem, *fb_problem);
    INFO("Parsing finished");
    INFO("Doing verification of domain and problem");
    if (!Verify(&domain, &problem))
        return 1;
    INFO("Translating");
    Task task = Translate(&domain, &problem);
    INFO("Cleanup");
    TRACE("Closing domain file");
    f_close(fb_domain);
    TRACE("Closing problem file");
    f_close(fb_problem);
    INFO("Done");
    return 0;
}
