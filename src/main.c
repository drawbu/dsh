#include <stdio.h>

#include "repl.h"
#include "status.h"

#ifndef TEST_MODE
int main(void)
{
    status_t repl_status;

    printf("Welcome to drawbu's shell\n");
    repl_status = repl_run();
    if (repl_status == FAILURE) {
        printf("Something went wrong\n");
        return 1;
    }
    return 0;
}
#endif