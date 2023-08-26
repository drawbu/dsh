#ifndef SHELL_H
    #define SHELL_H

    #include <stdbool.h>
    #include <stdlib.h>

    #include "args.h"
    #include "status.h"
    #include "env.h"

typedef struct {
    char *input;
    size_t len;
    args_t *args;
} input_t;

typedef struct {
    bool is_running;
    input_t *input;
    env_t *env;
} shell_t;

status_t shell_run(char **envp);
void shell_free(shell_t *shell);
shell_t *shell_init(char **envp);
int input_process(shell_t *shell);
void input_set(shell_t *shell, char *input, size_t len);
void input_free(input_t *input);
input_t *input_init(void);
void show_prompt(void);

#endif /* SHELL_H */
