#ifndef SHELL_H
    #define SHELL_H

    #include <stdbool.h>
    #include <stdlib.h>

    #include "status.h"
    #include "env.h"

typedef struct {
    char *input;
    size_t len;
} input_t;

typedef struct {
    bool is_running;
    input_t *input;
    env_t *env;
} shell_t;

status_t shell_run(char **envp);
int process_input(shell_t *shell);
void shell_free(shell_t *shell);
shell_t *shell_init(char **envp);

#endif /* SHELL_H */
