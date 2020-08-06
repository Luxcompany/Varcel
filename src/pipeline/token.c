#include <stdio.h>
#include <list.h>
#include <pipeline/token.h>

void token_list_print(List tokens){
    uint numTokens = list_size(tokens);
    const char *typeNames[] = {"Identifier", "Operator", "numLiteral", "strLiteral", "Unknown"};
    printf("NUM TOKENS: %i:\n", numTokens);

    for(uint i = 0; i < numTokens; ++i)
        printf("Type: %s    Contents: %s\n", typeNames[ ((Token *)list_get(tokens, i)) -> type ], ((Token *)list_get(tokens, i)) -> contents);
}