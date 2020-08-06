#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#include <log.h>
#include <token.h>
#include <lexer.h>

int main(){
    List srcTokenList = list_create(sizeof(Token));
    uint numTokens;
    lexer_parse_file("./res/test.vl", &srcTokenList, &numTokens);

    token_list_print(srcTokenList);

    lexer_clean_up();
    list_destroy(srcTokenList);

    return 0;
}