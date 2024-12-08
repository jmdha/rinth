#pragma once

#include <stdbool.h>

#include "misc.h"
#include "token.h"

void      lexer_init(const char *str);       // Initialise lexer
enum kind lexer_next(string *str);           // Find next token in stream
void      lexer_expect(enum kind kind);      // Execute lexer_next and abort if not correct token kind
void      lexer_expect_def(enum keyword kw); // Execute lexer_next and abort if not correct token kind

enum keyword keyword_match(const string *str); // Matches string to keyword
