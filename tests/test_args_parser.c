#include <stdint.h>

#include <criterion/criterion.h>

#include "args.h"

static
void assert_next_token(parser_t *parser, char *expected)
{
    char *token = parser_next_token(parser);

    if (expected != NULL)
        cr_assert_str_eq(token, expected);
    else
        cr_assert_null(token);
    if (token != NULL)
        free(token);
}

static
void assert_tokens(char *input, char **expected, uint32_t size)
{
    parser_t *parser = parser_init(input);

    for (uint32_t i = 0; i < size; i++)
        assert_next_token(parser, expected[i]);
    parser_free(parser);
}

Test(args_parser, single_token) {
    char *input = "foo";
    char *expected[] = {"foo"};

    assert_tokens(input, expected, 1);
}

Test(args_parser, multi_tokens) {
    char *input = "foo bar";
    char *expected[] = {"foo", "bar"};

    assert_tokens(input, expected, 2);
}

Test(args_parser, spaces) {
    char *input = "   foo  bar  ";
    char *expected[] = {"foo", "bar"};

    assert_tokens(input, expected, 2);
}

Test(args_parser, spaces_ending) {
    char *input = "   foo  bar  ";
    char *expected[] = {"foo", "bar", NULL};

    assert_tokens(input, expected, 3);
}

Test(args_parser, empty) {
    char *input = "";
    char *expected[] = {NULL};

    assert_tokens(input, expected, 1);
}

Test(args_parser, dual_quotes) {
    char *input = "hello \"foo bar\"";
    char *expected[] = {"hello", "foo bar", NULL};

    assert_tokens(input, expected, 3);
}

Test(args_parser, single_quotes) {
    char *input = "hello 'foo bar'";
    char *expected[] = {"hello", "foo bar", NULL};

    assert_tokens(input, expected, 3);
}

Test(args_parser, word_after_quotes) {
    char *input = "hello \"foo bar\" world";
    char *expected[] = {"hello", "foo bar", "world", NULL};

    assert_tokens(input, expected, 4);
}

Test(args_parser, multi_quotes) {
    char *input = "hello \"foo bar\" \" world\"";
    char *expected[] = {"hello", "foo bar", " world", NULL};

    assert_tokens(input, expected, 4);
}

Test(args_parser, trailing_quotes) {
    char *input = "echo \"foo bar\" world\" aaa\"a\"ah";
    char *expected[] = {"echo", "foo bar", "world\"", "aaa\"a\"ah"};

    assert_tokens(input, expected, 4);
}

Test(args_parser, ptr_reset) {
    parser_t *parser = parser_init("foo bar");

    assert_next_token(parser, "foo");
    parser_reset_ptr(parser);
    assert_next_token(parser, "foo");
    parser_free(parser);
}

Test(args_parser, always_empty) {
    parser_t *parser = parser_init("");
    char *expected_ptr = parser->ptr;

    assert_next_token(parser, NULL);
    cr_assert_eq(parser->ptr, expected_ptr);
    assert_next_token(parser, NULL);
    cr_assert_eq(parser->ptr, expected_ptr);
    parser_free(parser);
}

Test(args_parser, unterminated_quotes) {
    char *input = "\"";
    char *expected[] = {"\"", NULL};

    assert_tokens(input, expected, 1);
}
