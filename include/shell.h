#ifndef SHELL_H
    #define SHELL_H

    #include <stdbool.h>
    #include <stdlib.h>
    #include <limits.h>

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
    char path[PATH_MAX];
} shell_t;

status_t shell_run(char **envp);
void shell_free(shell_t *shell);
shell_t *shell_init(char **envp);
void input_set(shell_t *shell, char *input, size_t len);
void input_free(input_t *input);
input_t *input_init(void);
void show_prompt(void);
int execute_cmd(shell_t *shell);

#endif /* SHELL_H */
