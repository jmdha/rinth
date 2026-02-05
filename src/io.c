#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "io.h"
#include "mem.h"

struct file_buffer {
        int    fd;
        size_t len;
        char*  buf;
};

char** f_open(const char* path) {
        int    fd;
        size_t len;
        char*  buf;

        fd = open(path, O_RDONLY);
        if (fd == -1) {
                perror(path);
                exit(errno);
        }

        struct stat sb;
        if (fstat(fd, &sb) == -1) {
                perror(path);
                close(fd);
                exit(errno);
        }
        len = sb.st_size;
        buf = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
        if (buf == MAP_FAILED) {
                perror(path);
                close(fd);
                exit(errno);
        }

        struct file_buffer* fb = malloc_(sizeof(struct file_buffer));
        fb->fd                 = fd;
        fb->len                = len;
        fb->buf                = buf;
        return &fb->buf;
}

void f_close(char** buf) {
        struct file_buffer* fb =
            (struct file_buffer*)((char*)buf - offsetof(struct file_buffer, buf));
        munmap(fb->buf, fb->len);
        close(fb->fd);
        free(fb);
}
