#include "io.h"
#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

File FileOpen(const char *path) {
    File file = {.fd = -1, .buffer = NULL, .len = -1};
    file.fd   = open(path, O_RDONLY);
    if (file.fd == -1) {
        ERROR("Failed to open file \"%s\"", path);
        return file;
    }

    struct stat sb;
    if (fstat(file.fd, &sb) == -1) {
        ERROR("Failed to retrieve size of file \"%s\"", path);
        return file;
    }
    file.len    = sb.st_size;
    file.buffer = mmap(NULL, file.len, PROT_READ, MAP_PRIVATE, file.fd, 0);
    if (file.buffer == MAP_FAILED) {
        ERROR("Failed to map file content of \"%s\"", path);
        return file;
    }

    return file;
}

void FileClose(File *file) {
    munmap(file->buffer, file->len);
    close(file->fd);
}
