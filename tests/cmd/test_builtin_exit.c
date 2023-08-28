#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "shell.h"
#include "tests_utils.h"

Test(builtin_exit, no_args, .init = redirect) {
    shell_t *shell = run("exit");

    cr_assert_eq(shell->is_running, false);
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(builtin_exit, with_args, .init = redirect) {
    shell_t *shell = run("exit foo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}
