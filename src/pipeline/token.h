#ifndef VARCEL_TOKEN_H
#define VARCEL_TOKEN_H

#include <uint.h>
#include <list.h>

typedef enum {
    identifier,
    operator,
    numLiteral,
    strLiteral,
    unknown
} TokenType;

/** contains a TokenType and a string */
typedef struct {
    TokenType type;
    const char * contents;
} Token;

/**
 * @brief print contents of tokens to stdout.
 * @param tokens list of tokens
 */
void token_list_print(List tokens);

#endif //VARCEL_TOKEN_H