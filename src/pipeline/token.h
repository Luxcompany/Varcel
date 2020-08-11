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
    char * contents;
} Token;

/**
 * @brief print contents of tokens to stdout.
 * @param tokens list of tokens
 */
void token_list_print(List tokens);

/**
 * @brief allocate for content of token - use if content isn't being set to something that already exists
 * @param tokenPtr pointer to token to allocate for
 * @param numChars number of chars to allocate
 * @returns pointer to allocated data
 */
//char *token_content_alloc(Token *tokenPtr, uint numChars);

/**
 * @brief free the contents of a token
 * @param tokenPtr pointer to token to free
 */
//void token_content_free(Token *tokenPtr);

#endif //VARCEL_TOKEN_H