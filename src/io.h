#pragma once

#include <sys/stat.h>

typedef struct {
    int fd, len;
    char *buffer;
} File;

// Opens file and directs buffer to file content
// Remember to call close file!
File FileOpen(const char *path);
void FileClose(File *file);
