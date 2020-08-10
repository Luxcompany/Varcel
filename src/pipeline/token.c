#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <pipeline/token.h>

void token_list_print(List tokens){
    uint numTokens = list_size(tokens);
    const char *typeNames[] = {"Identifier", "Operator", "numLiteral", "strLiteral", "Unknown", "???"};
    printf("NUM TOKENS: %i:\n", numTokens);

    for(uint i = 0; i < numTokens; ++i){
        uint tokenType = list_get(tokens, i, Token).type;
        char *tokenContents = list_get(tokens, i, Token).contents;
        printf("Type: %s    Contents: %s\n", typeNames[tokenType], tokenContents);
    }
}

char *token_content_alloc(Token *tokenPtr, uint numChars){
    return (tokenPtr->contents=malloc(sizeof(char)*numChars));
}

void token_content_free(Token *tokenPtr){
    free(tokenPtr->contents);
}