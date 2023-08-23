#include <stdio.h>

#include "shell.h"
#include "status.h"

#ifndef TEST_MODE
int main(void)
{
    status_t repl_status;

    printf("Welcome to drawbu's shell\n");
    repl_status = shell_run();
    if (repl_status == FAILURE) {
        printf("Something went wrong\n");
        return 1;
    }
    return 0;
}
#endif