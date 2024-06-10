#include "lexer.h"
#include "token.h"
#include <criterion/assert.h>
#include <criterion/internal/assert.h>
#include <criterion/parameterized.h>

Test(lexing, lparen) { cr_assert_eq(TOKEN_LPAREN, LexerFirst("(").kind); }
Test(lexing, rparen) { cr_assert_eq(TOKEN_RPAREN, LexerFirst(")").kind); }
Test(lexing, space) { cr_assert_eq(TOKEN_WHITESPACE, LexerFirst(" ").kind); }
Test(lexing, tab) { cr_assert_eq(TOKEN_WHITESPACE, LexerFirst("\t").kind); }
Test(lexing, newline) { cr_assert_eq(TOKEN_WHITESPACE, LexerFirst("\n").kind); }
Test(lexing, carriage) { cr_assert_eq(TOKEN_WHITESPACE, LexerFirst("\r").kind); }
Test(lexing, eof) { cr_assert_eq(TOKEN_EOF, LexerFirst("\0").kind); }
Test(lexing, id) {
    cr_assert_eq(TOKEN_ID, LexerFirst("a").kind);
    cr_assert_eq(TOKEN_ID, LexerFirst("a2").kind);
    cr_assert_eq(TOKEN_ID, LexerFirst("a-b").kind);
    cr_assert_eq(TOKEN_ID, LexerFirst("a_c").kind);
    cr_assert_eq(TOKEN_ID, LexerFirst("a.q").kind);
    cr_assert_eq(TOKEN_ID, LexerFirst("a.q?").kind);
}
Test(lexing, variable) {
    cr_assert_eq(TOKEN_VARIABLE, LexerFirst("?a").kind);
    cr_assert_eq(TOKEN_VARIABLE, LexerFirst("?a2").kind);
    cr_assert_eq(TOKEN_VARIABLE, LexerFirst("?a-b").kind);
    cr_assert_eq(TOKEN_VARIABLE, LexerFirst("?a_c").kind);
    cr_assert_eq(TOKEN_VARIABLE, LexerFirst("?a.q").kind);
    cr_assert_eq(TOKEN_VARIABLE, LexerFirst("?a.q?").kind);
}
Test(lexing, predicate) { cr_assert_eq(TOKEN_DEF_PREDICATE, LexerFirst(":predicates").kind); }
Test(lexing, action) { cr_assert_eq(TOKEN_DEF_ACTION, LexerFirst(":action").kind); }
Test(lexing, parameters) { cr_assert_eq(TOKEN_DEF_PARAMETERS, LexerFirst(":parameters").kind); }
Test(lexing, precondition) {
    cr_assert_eq(TOKEN_DEF_PRECONDITION, LexerFirst(":precondition").kind);
}
Test(lexing, effect) { cr_assert_eq(TOKEN_DEF_EFFECT, LexerFirst(":effect").kind); }
