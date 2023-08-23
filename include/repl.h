#ifndef REPL_H
    #define REPL_H

    #include <stdbool.h>
    #include <stdlib.h>

    #include "status.h"

typedef struct {
    char *input;
    size_t len;
} input_t;

typedef struct {
    bool is_running;
    input_t *input;
} shell_t;

status_t repl_run(void);

#endif /* REPL_H */