#include "parse.h"

void parse_error(const char* str, size_t pos, const char* msg) {

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
