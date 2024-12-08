#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "lexer.h"
#include "log.h"

const char *STR;
uint POS;

static bool is_whitespace(char c) { return ((unsigned char)c) - 1 <= 32; }

static bool is_text(char c) { return c > 41; }

static void skip_whitespace() {
    while (is_whitespace(STR[POS]))
        POS++;
}

static void skip_text() {
    while (is_text(STR[POS]))
        POS++;
}

void lexer_init(const char *str) {
    STR = str;
    POS = 0;
}

enum kind lexer_next(string *str) {
    skip_whitespace();
    const uint pos = POS;
    str->ptr       = &STR[POS];
    switch (STR[POS++]) {
    case '\0': return KIND_EOI;
    case '(': return KIND_LPAREN;
    case ')': return KIND_RPAREN;
    default:
        skip_text();
        str->len = POS - pos;
        return KIND_ID;
    }
}

void lexer_expect(enum kind kind) {
    enum kind actual;
    string tmp;
    if ((actual = lexer_next(&tmp)) != kind) {
        ERROR("Expected token %s found %s", KIND_NAMES[kind], KIND_NAMES[actual]);
        abort();
    }
}

void lexer_expect_def(enum keyword kw) {
    enum kind kind;
    string tmp;
    if ((kind = lexer_next(&tmp)) != KIND_ID) {
        ERROR("Expected keyword %s found token %s", KEYWORD_NAMES[kw], KIND_NAMES[kind]);
        abort();
    }
    enum keyword actual;
    if ((actual = keyword_match(&tmp)) != kw) {
        ERROR("Expected keyword %s found %s", KEYWORD_NAMES[kw], KEYWORD_NAMES[actual]);
        abort();
    }
}

enum keyword keyword_match(const string *str) {
    if (str->len == 6 && strncmp("define", str->ptr, str->len) == 0)
        return KEYWORD_DEFINE;
    else if (str->len == 6 && strncmp("domain", str->ptr, str->len) == 0)
        return KEYWORD_NAME;
    else if (str->len == 7 && strncmp("problem", str->ptr, str->len) == 0)
        return KEYWORD_NAME;
    else if (str->len == 3 && strncmp("and", str->ptr, str->len) == 0)
        return KEYWORD_AND;
    else if (str->len == 2 && strncmp("or", str->ptr, str->len) == 0)
        return KEYWORD_OR;
    else if (str->len == 3 && strncmp("not", str->ptr, str->len) == 0)
        return KEYWORD_NOT;
    else if (str->len == 13 && strncmp(":requirements", str->ptr, str->len) == 0)
        return KEYWORD_REQUIREMENTS;
    else if (str->len == 11 && strncmp(":predicates", str->ptr, str->len) == 0)
        return KEYWORD_PREDICATES;
    else if (str->len == 7 && strncmp(":action", str->ptr, str->len) == 0)
        return KEYWORD_ACTION;
    else if (str->len == 11 && strncmp(":parameters", str->ptr, str->len) == 0)
        return KEYWORD_PARAMETERS;
    else if (str->len == 13 && strncmp(":precondition", str->ptr, str->len) == 0)
        return KEYWORD_PRECONDITION;
    else if (str->len == 7 && strncmp(":effect", str->ptr, str->len) == 0)
        return KEYWORD_EFFECT;
    else if (str->len == 7 && strncmp(":domain", str->ptr, str->len) == 0)
        return KEYWORD_DOMAIN;
    else if (str->len == 8 && strncmp(":objects", str->ptr, str->len) == 0)
        return KEYWORD_OBJECTS;
    else if (str->len == 5 && strncmp(":init", str->ptr, str->len) == 0)
        return KEYWORD_INIT;
    else if (str->len == 5 && strncmp(":goal", str->ptr, str->len) == 0)
        return KEYWORD_GOAL;
    return MAX_KEYWORD;
}
