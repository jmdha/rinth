#pragma once

#include <sys/stat.h>

char** f_open(const char *path);
void   f_close(char** fb);
