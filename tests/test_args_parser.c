#include <criterion/criterion.h>

#include "args.h"

Test(args_parser, single_token) {
    parser_t *parser = parser_init("foo");

    cr_assert_str_eq(parser_next_token(parser), "foo");
}

Test(args_parser, multi_tokens) {
    parser_t *parser = parser_init("foo bar");

    cr_assert_str_eq(parser_next_token(parser), "foo");
    cr_assert_str_eq(parser_next_token(parser), "bar");
}

Test(args_parser, spaces) {
    parser_t *parser = parser_init("   foo  bar  ");

    cr_assert_str_eq(parser_next_token(parser), "foo");
    cr_assert_str_eq(parser_next_token(parser), "bar");
}

Test(args_parser, spaces_ending) {
    parser_t *parser = parser_init("   foo  bar  ");

    cr_assert_str_eq(parser_next_token(parser), "foo");
    cr_assert_str_eq(parser_next_token(parser), "bar");
    cr_assert_null(parser_next_token(parser));
}

Test(args_parser, empty) {
    parser_t *parser = parser_init("");

    cr_assert_null(parser_next_token(parser));
}

Test(args_parser, dual_quotes) {
    parser_t *parser = parser_init("hello \"foo bar\"");

    cr_assert_str_eq(parser_next_token(parser), "hello");
    cr_assert_str_eq(parser_next_token(parser), "foo bar");
    cr_assert_null(parser_next_token(parser));
}

Test(args_parser, single_quotes) {
    parser_t *parser = parser_init("hello 'foo bar'");

    cr_assert_str_eq(parser_next_token(parser), "hello");
    cr_assert_str_eq(parser_next_token(parser), "foo bar");
    cr_assert_null(parser_next_token(parser));
}

Test(args_parser, word_after_quotes) {
    parser_t *parser = parser_init("hello \"foo bar\" world");

    cr_assert_str_eq(parser_next_token(parser), "hello");
    cr_assert_str_eq(parser_next_token(parser), "foo bar");
    cr_assert_str_eq(parser_next_token(parser), "world");
    cr_assert_null(parser_next_token(parser));
}

Test(args_parser, multi_quotes) {
    parser_t *parser = parser_init("hello \"foo bar\" \" world\"");

    cr_assert_str_eq(parser_next_token(parser), "hello");
    cr_assert_str_eq(parser_next_token(parser), "foo bar");
    cr_assert_str_eq(parser_next_token(parser), " world");
    cr_assert_null(parser_next_token(parser));
}

Test(args_parser, ptr_reset) {
    parser_t *parser = parser_init("foo bar");

    cr_assert_str_eq(parser_next_token(parser), "foo");
    parser_reset_ptr(parser);
    cr_assert_str_eq(parser_next_token(parser), "foo");
}

Test(args_parser, always_empty) {
    parser_t *parser = parser_init("");
    char *expected_ptr = parser->ptr;

    cr_assert_null(parser_next_token(parser));
    cr_assert_eq(parser->ptr, expected_ptr);
    cr_assert_null(parser_next_token(parser));
    cr_assert_eq(parser->ptr, expected_ptr);
}