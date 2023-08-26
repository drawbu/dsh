#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "shell.h"

void show_prompt(void)
{
    char host[64];

    gethostname(host, 64);
    printf("%s@%s:%s > ", getenv("USER"), host, getenv("PWD"));
}
