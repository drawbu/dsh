#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "shell.h"
#include "tests_utils.h"

Test(not_found, foo, .init = redirect) {
    shell_t *shell = run("foo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("foo: not found\n");
    shell_free(shell);
}
