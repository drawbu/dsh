#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
    bool is_quoted = false;
    size_t size = 0;

    if (parser == NULL)
        return NULL;
    skip_whitespaces(parser);
    start = parser->ptr;
    end = parser->ptr;
    switch (*(parser->ptr)) {
        case '\0':
            return NULL;
        case '"':
            end = strpbrk(++start, "\"");
            is_quoted = true;
            break;
        case '\'':
            end = strpbrk(++start, "'");
            is_quoted = true;
            break;
        default:
            end = strpbrk(start, " ");
            break;
    }
    size = strlen(start);
    if (size == 0)
        return NULL;
    if (end == NULL)
        end = start + size;
    parser->ptr += end - start + is_quoted;
    token = create_token(start, end);
    if (token == NULL)
        return NULL;
    return token;
}

void parser_reset_ptr(parser_t *parser)
{
    parser->ptr = parser->input;
}