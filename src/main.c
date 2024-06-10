#include "lexer.h"
#include "token.h"
#include <stdio.h>

int main(void) {
    const char *STR = "( \t\n :action )";
    Lexer lexer     = LexerInit();
    Token token     = LexerNext(&lexer, STR);

    while (token.kind != TOKEN_EOF) {
        printf("%s: %d - %d\n", TOKEN_NAME[token.kind], token.pos, token.len);
        token = LexerNext(&lexer, STR);
    }
    return 0;
}
