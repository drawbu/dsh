#include <stdlib.h>
#include <stdint.h>

#include "env.h"

static
uint32_t get_envp_size(char **envp)
{
    uint32_t size = 0;

    while (envp[size] != NULL)
        size++;
    return size - 1;
}

env_t *env_init(char **envp)
{
    env_t *env = NULL;

    if (envp == NULL)
        return NULL;
    env = malloc(sizeof(*env));
    *env = (env_t){
        .variables = envp,
        .size = get_envp_size(envp),
    };
    return env;
}

void env_free(env_t *env)
{
    if (env == NULL)
        return;
    free(env);
}