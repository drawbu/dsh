#ifndef ENV_H
    #define ENV_H

    #include <stdint.h>

typedef struct {
    char **variables;
    uint32_t size;
} env_t;

env_t *env_init(char **envp);
void env_free(env_t *env);

#endif /* ENV_H */