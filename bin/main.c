#include "io.h"
#include "log.h"
#include "problem.h"

int main(int argc, char **argv) {
    LogInit();
    INFO("Reading args");
    if (argc < 2) return 1;
    const char *path = argv[1];
    INFO("Opening file");
    File file       = FileOpen(path);
    INFO("Parsing problem");
    Problem problem = ProblemParse(file.buffer);
    // ProblemPrint(&problem);
    INFO("Closing file");
    FileClose(&file);
    INFO("Deleting problem");
    ProblemDelete(&problem);
    return 0;
}
