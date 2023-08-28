#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "shell.h"
#include "args.h"

static
void redirect(void)
{
    cr_redirect_stdout();
    setbuf(stdout, NULL);
    cr_redirect_stderr();
    setbuf(stderr, NULL);
}

static
shell_t *run(char *input)
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
    shell_t *shell = run("exit");

    cr_assert_eq(shell->is_running, false);
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(process_input, foo_not_found, .init = redirect) {
    shell_t *shell = run("foo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("foo: not found\n");
    shell_free(shell);
}


Test(process_input, echo_foo, .init = redirect) {
    shell_t *shell = run("echo foo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("foo\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(process_input, echo_hello_world, .init = redirect) {
    shell_t *shell = run("echo 'hello world'");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("hello world\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(process_input, echo_empty, .init = redirect) {
    shell_t *shell = run("echo");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}

Test(process_input, echo_multi_args, .init = redirect) {
    shell_t *shell = run("echo 'foo' bar");

    cr_assert_eq(shell->is_running, true);
    cr_assert_stdout_eq_str("foo bar\n");
    cr_assert_stderr_eq_str("");
    shell_free(shell);
}