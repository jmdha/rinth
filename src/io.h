#pragma once

#include <sys/stat.h>

typedef char** fbuf;

fbuf f_open(const char *path);
void f_close(fbuf buf);
