#pragma once

#include <sys/stat.h>

int  f_open(char*** fb, const char *path);
void f_close(char** fb);
