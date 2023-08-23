#include <stdbool.h>
#include <stdint.h>

#include <criterion/criterion.h>

#include "args.h"

static
void assert_str_arrays(args_t *expected, args_t *result)
{
    if (result == NULL)
        cr_skip("NULL pointer");
    cr_assert_eq(
        expected->argc, 
        result->argc,
        "Expected %lu arguments, but got %lu\n",
        expected->argc, 
        result->argc
    );
    for (uint32_t i = 0; i < expected->argc; i++)
        cr_assert_str_eq(
            expected->argv[i], 
            result->argv[i],
            "For argument %d,\nexpected: '%s', \ngot:      '%s'\n",
            i,
            expected->argv[i], 
            result->argv[i]
        );
}

Test(args, hello) {
    char input[] = "hello";
    char *argv[] = {
        "hello",
    };
    args_t expected = {
        .argv = argv,
        .argc = 1,
    };
    args_t *result = get_args(input);

    assert_str_arrays(&expected, result);
}

Test(args, hello_world) {
    char input[] = "hello world";
    char *argv[] = {
        "hello", "world",
    };
    args_t expected = {
        .argv = argv,
        .argc = 2,
    };
    args_t *result = get_args(input);

    assert_str_arrays(&expected, result);
}

Test(args, spaces) {
    char input[] = " hello  world ";
    char *argv[] = {
        "hello", "world",
    };
    args_t expected = {
        .argv = argv,
        .argc = 2,
    };
    args_t *result = get_args(input);

    assert_str_arrays(&expected, result);
}

Test(args, empty) {
    char input[] = "";
    char *argv[] = {
    };
    args_t expected = {
        .argv = argv,
        .argc = 0,
    };
    args_t *result = get_args(input);

    assert_str_arrays(&expected, result);
}

Test(args, double_quotes) {
    char input[] = "echo \"hello world\"";
    char *argv[] = {
        "echo", "hello world",
    };
    args_t expected = {
        .argv = argv,
        .argc = 2,
    };
    args_t *result = get_args(input);

    assert_str_arrays(&expected, result);
}

Test(args, single_quotes) {
    char input[] = "echo 'hello world'";
    char *argv[] = {
        "echo", "hello world",
    };
    args_t expected = {
        .argv = argv,
        .argc = 2,
    };
    args_t *result = get_args(input);

    assert_str_arrays(&expected, result);
}