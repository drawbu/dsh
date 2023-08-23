#ifndef REPL_H
    #define REPL_H

    #include <stdbool.h>

    #include "status.h"

typedef struct {
    bool is_running;
} shell_t;

status_t repl_run(void);

#endif /* REPL_H */