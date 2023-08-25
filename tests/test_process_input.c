#include <stddef.h>
#include <stdlib.h>

#include <criterion/criterion.h>

#include "shell.h"
#include "args.h"

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
    input_process(shell);
    return shell;
}

Test(process_input, exit) {
    shell_t *shell = fake_input("exit");

    cr_assert_eq(shell->is_running, false);
    shell_free(shell);
}

Test(process_input, foo) {
    shell_t *shell = fake_input("foo");

    cr_assert_eq(shell->is_running, true);
    shell_free(shell);
}
