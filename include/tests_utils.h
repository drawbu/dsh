#ifndef TESTS_UTILS_H
    #define TESTS_UTILS_H

    #include <criterion/criterion.h>
    #include <criterion/redirect.h>

    #include "shell.h"

static inline
void redirect(void)
{
    cr_redirect_stdout();
    setbuf(stdout, NULL);
    cr_redirect_stderr();
    setbuf(stderr, NULL);
}

static inline
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


#endif /* TESTS_UTILS_H */
