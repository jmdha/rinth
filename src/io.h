#pragma once

// Open file via mmap
char** f_open(
	const char *path
);

// Close file
void f_close(
	char** fb
);
