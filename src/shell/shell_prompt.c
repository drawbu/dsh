#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

void show_prompt(shell_t *shell)
{
    printf("%s@%s:%s > ", getenv("USER"), getenv("HOST"), getenv("PWD"));
}
