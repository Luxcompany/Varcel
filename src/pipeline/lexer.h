#ifndef VARCEL_LEXER_H
#define VARCEL_LEXER_H

#include <list.h>
#include <uint.h>

/**
 * @brief Parse a string into tokens
 * @param srcStr input string to parse
 * @param srcStrLength Length of input string in number of chars
 * @param srcTokenListPtr pointer to initialized list of tokens
 * @param numTokens pointer to int for writing the total number of processed tokens
 */
void lexer_parse_string(const char *srcStr, uint srcStrLength, List *srcTokenListPtr, uint *numTokens);

/**
 * @brief Parse a file into tokens
 * @param srcFile input file to parse
 * @param srcTokenList pointer to initialized list of tokens
 * @param srcTokenList pointer to int for writing the total number of processed tokens
 */
void lexer_parse_file(const char *srcFilepath, List *srcTokenList, uint *numTokens);

/**
 * @brief To be called once lexer will no longer be used -- frees memory
 * @param numTokens number of tokens supplied
 * @param ... pointers to token lists
 */
void lexer_clean_up(uint numTokens, ...);

#endif //VARCEL_LEXER_H