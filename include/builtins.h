#ifndef BUILTINS_H
    #define BUILTINS_H

    #include <stdint.h>

    #include "args.h"
    #include "shell.h"

    #define BUILTINS_COUNT ((uint32_t)1)

int builtin_exit(shell_t *shell,args_t *args);

typedef struct {
    const char *name;
    int (* func)(shell_t *, args_t *);
} builtin_t;

static
const builtin_t BUILTINS_TABLE[BUILTINS_COUNT] = {
    {"exit", &builtin_exit},
};

#endif /* BUILTINS_H */
