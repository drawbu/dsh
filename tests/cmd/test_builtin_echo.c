#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "shell.h"
#include "tests_utils.h"

Test(builtin_echo, foo, .init = redirect) {
    shell_t *shell = run("echo foo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("foo\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(builtin_echo, hello_world, .init = redirect) {
    shell_t *shell = run("echo 'hello world'");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("hello world\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(builtin_echo, empty, .init = redirect) {
    shell_t *shell = run("echo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(builtin_echo, multi_args, .init = redirect) {
    shell_t *shell = run("echo 'foo' bar");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("foo bar\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}
