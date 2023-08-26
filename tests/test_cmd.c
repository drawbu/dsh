#include <stddef.h>
#include <stdlib.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "shell.h"
#include "args.h"

static
void redirect(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static
shell_t *fake_input(char *input)
{
    char *env[] = {NULL};
    shell_t *shell = shell_init(env);

    shell->is_running = true;
    *shell->input = (input_t){
        .input = strdup(input),
        .len = strlen(input),
        .args = args_get(input),
    };
    execute_cmd(shell);
    return shell;
}

Test(process_input, exit, .init = redirect) {
    shell_t *shell = fake_input("exit");

    cr_assert_eq(shell->is_running, false);
    cr_assert_stdout_eq_str("");
    shell_free(shell);
}

Test(process_input, foo_not_found, .init = redirect) {
    shell_t *shell = fake_input("foo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stderr_eq_str("foo: not found\n");
    shell_free(shell);
}
