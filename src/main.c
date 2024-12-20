#include "io.h"
#include "log.h"
#include "parse.h"
#include "task.h"

int main(int, char **argv) {
    log_init();
    const char* path_domain = argv[1];
    const char* path_problem = argv[2];
    char** fbuf_domain = f_open(path_domain);
    char** fbuf_problem = f_open(path_problem);
    struct domain domain = parse_domain(*fbuf_domain);
    struct problem problem = parse_problem(*fbuf_problem);
    struct task task = translate(&domain, &problem);
    domain_free(&domain);
    f_close(fbuf_domain);
    f_close(fbuf_problem);
    task_free(&task);
    return 0;
}
