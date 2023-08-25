#include <stdbool.h>
#include <stdint.h>

#include <criterion/criterion.h>

#include "args.h"

static
void assert_args(char **argv, size_t argc, char *input)
{
    args_t expected = {
        .argv = argv,
        .argc = argc,
    };
    args_t *result = args_get(input);

    if (result == NULL)
        cr_skip("args == NULL");
    cr_assert_eq(
        expected.argc,
        result->argc,
        "Expected %lu arguments, but got %lu\n",
        expected.argc,
        result->argc
    );
    if (result->argv == NULL)
        cr_skip("args->argv == NULL");
    for (uint32_t i = 0; i < expected.argc; i++) {
        if (result->argv == NULL)
            cr_skip("args->argv[%d] == NULL", i);
        cr_assert_str_eq(
            expected.argv[i],
            result->argv[i],
            "For argument %d,\nexpected: '%s', \ngot:      '%s'\n",
            i,
            expected.argv[i],
            result->argv[i]
        );
    }
    args_free(result);
}

Test(args, hello) {
    char input[] = "hello";
    size_t argc = 1;
    char *argv[] = {
        "hello",
    };

    assert_args(argv, argc, input);
}

Test(args, hello_world) {
    char input[] = "hello world";
    size_t argc = 2;
    char *argv[] = {
        "hello", "world",
    };

    assert_args(argv, argc, input);
}

Test(args, spaces) {
    char input[] = " hello  world ";
    size_t argc = 2;
    char *argv[] = {
        "hello", "world",
    };

    assert_args(argv, argc, input);
}

Test(args, empty) {
    char input[] = "";
    size_t argc = 0;
    char *argv[] = {
    };

    assert_args(argv, argc, input);
}

Test(args, double_quotes) {
    char input[] = "echo \"hello world\"";
    size_t argc = 2;
    char *argv[] = {
        "echo", "hello world",
    };

    assert_args(argv, argc, input);
}

Test(args, single_quotes) {
    char input[] = "echo 'hello world'";
    size_t argc = 2;
    char *argv[] = {
        "echo", "hello world",
    };

    assert_args(argv, argc, input);
}

Test(args, unterminated_quote) {
    char input[] = "\"";
    size_t argc = 1;
    char *argv[] = {
        "\"",
    };

    assert_args(argv, argc, input);
}
