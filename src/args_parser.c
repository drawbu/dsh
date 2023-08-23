#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "args.h"

parser_t *parser_init(char *input)
{
    parser_t *parser = malloc(sizeof(*parser));

    if (parser == NULL)
        return NULL;
    *parser = (parser_t){
        .input = strdup(input),
        .ptr = NULL,
    };
    if (parser->input == NULL) {
        parser_free(parser);
        return NULL;
    }
    parser->ptr = parser->input;
    return parser;
}

void parser_free(parser_t *parser)
{
    if (parser == NULL)
        return;
    if (parser->input != NULL)
        free(parser->input);
    free(parser);
}

static
void skip_whitespaces(parser_t *parser)
{
    while (*(parser->ptr) == ' ')
        (parser->ptr)++;
}

static
char *create_token(char *start, char *end)
{
    size_t size = 0;

    while ((start + size) != end)
        size++;
    return strndup(start, size);
}

char *parser_next_token(parser_t *parser)
{
    char *start = NULL;
    char *end = NULL;
    char *token = NULL;

    skip_whitespaces(parser);
    start = parser->ptr;
    end = parser->ptr;
    switch (*(parser->ptr)) {
        case '\0':
            return NULL;
        case '"':
            end = strpbrk(++start, "\"");
            break;
        case '\'':
            end = strpbrk(++start, "'");
            break;
        default:
            end = strpbrk(start, " ");
            if (end == NULL)
                end = start + strlen(start);
            break;
    }
    token = create_token(start, end);
    if (token == NULL)
        return NULL;
    parser->ptr += strlen(token);
    return token;
}