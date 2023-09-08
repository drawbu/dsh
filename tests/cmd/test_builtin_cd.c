#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "shell.h"
#include "tests_utils.h"

static
void assert_path(shell_t *shell, char *path_expect)
{
    cr_assert_str_eq(shell->path, path_expect);
    cr_assert_str_eq(shell->path, getenv("PWD"));
}

Test(builtin_cd, no_args, .init = redirect) {
    shell_t *shell = run("cd");

    cr_assert_eq(shell->is_running, true);
    assert_path(shell, getenv("HOME"));
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(builtin_cd, too_many_args, .init = redirect) {
    char *pwd = getenv("PWD");
    shell_t *shell = run("cd foo bar");

    cr_assert_eq(shell->is_running, true);
    assert_path(shell, pwd);
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("cd: too many arguments\n");
    shell_free(shell);
}

Test(builtin_cd, root, .init = redirect) {
    shell_t *shell = run("cd /");

    cr_assert_eq(shell->is_running, true);
    assert_path(shell, "/");
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}
