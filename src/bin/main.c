#include "io.h"
#include "log.h"
#include "parse/domain.h"
#include "parse/problem.h"
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
    File domain_file = FileOpen(domain_path);
    INFO("Parsing domain");
    Domain domain;
    DomainParse(&domain, domain_file.buffer);
    INFO("Opening problem file");
    File problem_file = FileOpen(problem_path);
    INFO("Parsing problem");
    Problem problem;
    ProblemParse(&problem, problem_file.buffer);
    INFO("Parsing finished");
    INFO("Translating");
    Task task = Translate(&domain, &problem);
    INFO("Cleanup");
    TRACE("Closing domain file");
    FileClose(&domain_file);
    TRACE("Closing problem file");
    FileClose(&problem_file);
    INFO("Done");
    return 0;
}
