#ifndef VARCEL_TOKEN_H
#define VARCEL_TOKEN_H

#include <list.h>

typedef unsigned int uint;

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
static void token_list_print(List tokens){
    uint numTokens = list_size(tokens);
    const char *typeNames[] = {"Identifier", "Operator", "numLiteral", "strLiteral", "Unknown"};
    printf("NUM TOKENS: %i:\n", numTokens);

    for(uint i = 0; i < numTokens; ++i)
        printf("Type: %s    Contents: %s\n", typeNames[ ((Token *)list_get(tokens, i)) -> type ], ((Token *)list_get(tokens, i)) -> contents);
}

#endif