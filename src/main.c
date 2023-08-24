#include <stdio.h>
#include <stdint.h>

#include "shell.h"
#include "status.h"

#ifndef TEST_MODE
int main(int argc, char **argv, char **envp)
{
    status_t repl_status;

    printf("Welcome to drawbu's shell\n");
    repl_status = shell_run(envp);
    if (repl_status == FAILURE) {
        printf("Something went wrong\n");
        return 1;
    }
    return 0;
}
#endif
