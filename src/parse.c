#include <stdio.h>
#include <stdlib.h>

#include "parse.h"

#define RED   "\x1b[31m"
#define RESET "\x1b[0m"

size_t line_start(const char* str, size_t pos) {
	while (pos > 0 && str[pos - 1] != '\n')
		pos--;
	return pos;
}

size_t line_end(const char* str, size_t pos) {
	while (str[pos] != '\0' && str[pos] != '\n')
		pos++;
	return pos;
}

void parse_error(const char* str, size_t pos, const char* msg, size_t len) {
	size_t start  = line_start(str, pos);
	size_t end    = line_end(str, pos);
	size_t offset = pos - start;
	fprintf(
		stderr,
		"parse failed: char %zu\n",
		pos + 1
	);
	fprintf(
		stderr,
		"%.*s%s%.*s%s%.*s\n",
		(int) (pos - start), &str[start],
		RED, (int) (len), &str[pos], RESET,
		(int) (end - pos - len), &str[pos + len]
	);
	fprintf(
		stderr,
		"%*s%s %s\n",
		(int) offset,
		"",
		"|",
		msg
	);
	exit(33);
}

bool is_whitespace(char c) {
        return ((unsigned char)c) - 1 <= 32;
}

bool is_text(char c) {
        return c > 41;
}

void skip_whitespace(const char** s) {
	while (is_whitespace(**s))
		(*s)++;
}

void skip_text(const char** s) {
	while (is_text(**s))
		(*s)++;
}

void next_line(const char** s) {
	while (**s != '\n')
		(*s)++;
}
