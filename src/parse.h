#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

void parse_error(const char* str, size_t pos, const char* msg);

bool is_whitespace(char c);
bool is_text(char c);
void skip_whitespace(const char** s);
void skip_text(const char** s);
void next_line(const char** s);

#endif
