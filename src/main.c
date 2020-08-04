#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>
#define VARCEL_LOG_DEBUG_
#include <log.h>

typedef unsigned int uint;

/*
IM OKAY WITH HARDCODING LEXEMES - THEY SHOULDN'T BE CHANGED ANYWAYS
*/
const char whitespaceChars[] = " \n\t";

const char *operators[] = { //layout: (op-char, subops), ...
"+", "+=", "++", "-", "-=", "--", "*", "*=", "**", "/", "/=", "%", "%=", "=", "==",
">", ">=", ">>", "<", "<=", "<<", "&", "&&", "|", "||", "^", "^^", "~", "!", "!=",
".", "..", "@", "?", ":", "(", ")", "[", "]", ","
};

//Generated at runtime
List specialChars;
List operatorTree;

const char *keywords[] = {
"arr", "sizeof", "const", "mut", "freeze", "uses", "hidden",
"strict", "istype", "alias", "real", "int", "lambda", "amb",
"string"
};

typedef enum {
    identifier,
    keyword,
    operator,
    numLiteral,
    strLiteral
} TokenType;

typedef struct {
    TokenType type;
    const char * contents;
} Token;

/**
 * @brief compare a subsection of str1 to all of str2 - used in token_tokenize_string
 */
static uint str_compare(const char *str1, uint str1Offset, uint numCharsToCompare, const char *str2){
    for(uint i = 0; (str2[i] != '\0')&&(i < numCharsToCompare); ++i)
        if(str1[str1Offset + i] != str2[i] || str1[str1Offset + i] == '\0')
            return 0;
    return 1;
}

/**
 * @brief Parse a string into tokens
 * @param srcStr input string to parse
 * @param srcStrLength Length of input string (number of chars)
 * @param srcTokenListPtr pointer to initialized list of tokens
 * @param numTokens pointer to int for writing the total number of processed tokens
 */
void token_tokenize_string(const char *srcStr, uint srcStrLength, List *srcTokenListPtr, uint *numTokens){
    if(!(*srcTokenListPtr)){logE("TOKEN LIST ISN'T INITIALIZED"); return;}

    Token *t = list_append(srcTokenListPtr); t->type = identifier; t->contents = "first";
    
    uint curIndex = 0;   //index of begining of current token
    uint curOffset = 0;  //offset of index to read srcStr from curIndex to curOffset
    for(; curOffset < srcStrLength; ++curOffset){
        
    }

    *numTokens = list_size(*srcTokenListPtr);
    logI("Finished parsing string of %i tokens with %i left over possible tokens", *numTokens, list_possible_size(*srcTokenListPtr));
}

/**
 * @brief Parse a file into tokens
 * @param srcFile input file to parse
 * @param srcTokenList pointer to initialized list of tokens
 * @param srcTokenList pointer to int for writing the total number of processed tokens
 */
void token_tokenize_file(const char *srcFilepath, List *srcTokenList, uint *numTokens){
    FILE *srcFile;
    srcFile = fopen(srcFilepath, "r");  //open source file for reading

    if(srcFile != NULL){
        fseek(srcFile, 0, SEEK_END);    //set cur to end of file to read file size below
        uint srcSize = ftell(srcFile);   //set srcSize to number of characters in file
        char* srcStr = malloc(srcSize); //store source string on heap

        fseek(srcFile, 0, SEEK_SET);    
        fread(srcStr, 1, srcSize, srcFile); //read source file contents into source string

        token_tokenize_string(srcStr, srcSize, srcTokenList, numTokens); //feed source string into tokenization function
        free(srcStr);
    }else{
        logE("CAN'T OPEN FILE: %s", srcFilepath);  //print error if file cant be opened
    }

    fclose(srcFile);
}

/**
 * @brief print contents of tokens to stdout.
 * PRECONDITION: tokens up to numTokens is initialized with tokens
 * @param tokens pointer to begnining of tokens in memory
 * @param numTokens number of consecutive tokens
 */
void token_list_print(List tokens, uint numTokens){
    const char *typeNames[] = {"Identifier", "Keyword", "Operator", "numLiteral", "strLiteral"};
    printf("NUM TOKENS: %i:\n", numTokens);
    for(uint i = 0; i < numTokens; ++i)
        printf("Type: %s    Contents: %s\n", typeNames[ ((Token *)list_get(tokens, i)) -> type ], ((Token *)list_get(tokens, i)) -> contents);
}

static void operator_tree_init(){
    specialChars = list_create(sizeof(const char *));
    operatorTree = list_create(sizeof(List));
    int lastBaseOp = -1;
    for(size_t i = 0; i < sizeof(operators)/sizeof(char *); ++i){
        if(strlen(operators[i]) == 1){
            ++lastBaseOp;
            List *opGroup = (List *)list_append(&operatorTree);
            *opGroup = list_create(sizeof(const char *));

            const char **baseOp = (const char **)list_append(opGroup);
            *baseOp = operators[i];

            const char **specialChar = (const char **)list_append(&specialChars);
            *specialChar = operators[i];
        }else{
            List *opGroup = (List *)list_get(operatorTree, lastBaseOp);
            const char **subOp = (const char **)list_append(opGroup);

            *subOp = operators[i];
        }
    }
}

void operator_tree_destroy(){
    for(size_t i=0; i<list_size(operatorTree); ++i){
        List *opGroup = (List *)list_get(operatorTree, i);
        list_destroy(*opGroup);
    }
    list_destroy(operatorTree);
}

static void operator_tree_print(){
    for(size_t i=0; i<list_size(operatorTree); ++i){
        List *opGroup = (List *)list_get(operatorTree, i);
        for(size_t i=0; i<list_size(*opGroup); ++i){
            char **op = list_get(*opGroup, i);
            if(!i){
                printf("%s\n",*op);
            }else{
                printf("\t%s\n",*op);
            }
        }
    }
}

int main(){
    operator_tree_init();
    //operator_tree_print();

    List srcTokenList = list_create(sizeof(Token));
    uint numTokens;
    token_tokenize_file("./res/test.vl", &srcTokenList, &numTokens);

    token_list_print(srcTokenList, numTokens);

    list_destroy(srcTokenList);
    operator_tree_destroy();

    return 0;
}