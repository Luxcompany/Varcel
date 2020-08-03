#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
IM OKAY WITH HARDCODING LEXEMES - THEY SHOULDN'T BE CHANGED ANYWAYS
*/
const char whitespaceChars[] = " \n\t";

const char *operators[] = {
"+", "+=", "++", "-", "-=", "--", "*", "*=", "**", "/", "/=", "%", "%=", "=", "==",
">", ">=", ">>", "<", "<=", "<<", "&", "&&", "|", "||", "^", "^^", "~", "!", "!=",
".", "..", "@", "?", ":", "(", ")", "[", "]", ","
};

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
static int str_compare(const char* str1, int str1Offset, int str1MaxChars, const char* str2){
    for(int i = 0; str2[i] != '\0'; ++i)
        if(str1[str1Offset + i] != str2[i] || i > str1MaxChars || str1[str1Offset + i] == '\0')
            return 0;
    return 1;
}

/**
 * @brief Appends token to end of srcTokenArr; expands srcTokenArr if not big enough - used in token_tokenize_string
 * @param srcTokenArrPtr pointer to array of tokens
 * @param numTokens pointer to int for updating number of tokens
 * @param numPossibleTokens pointer to int for updating size of array
 * @return pointer to first empty token in srcTokenArr, or NULL
 */
static Token *token_append(Token **srcTokenArrPtr, int *numTokens, int *numPossibleTokens){
    if( (*numTokens)+1 > *numPossibleTokens ){
        //DOUBLE SIZE OF srcTokenArr
        int newSize = (*numPossibleTokens)*2;
        Token *newSrcTokens = realloc(*srcTokenArrPtr, newSize*sizeof(Token));
        //ADD ONE TO SIZE OF srcTokenArr IF DOUBLING FAILS
        if(!newSrcTokens){
            newSize = (*numPossibleTokens) + 1;
            newSrcTokens = realloc(*srcTokenArrPtr, newSize*sizeof(Token));

            if(!newSrcTokens){
                fprintf(stderr, "[ERR] CAN'T EXPAND SRC TOKENS TO SIZE OF %i TOKENS\n", *numPossibleTokens);
                return NULL;
            }
        }

        printf("[INFO] Expanded numPossibleTokens to %i\n", newSize);

        *numPossibleTokens = newSize;
        *srcTokenArrPtr = newSrcTokens;
    }

    //Return pointer to first empty token in srcTokenArr, and increment token counter
    ++(*numTokens);
    return &((*srcTokenArrPtr)[(*numTokens)-1]);
}

/**
 * @brief Parse a string into tokens
 * @param srcStr input string to parse
 * @param srcStrLength Length of input string (number of chars)
 * @param srcTokenArrPtr pointer to NULL
 * @param numTokens pointer to int for writing the total number of processed tokens
 */
void token_tokenize_string(const char *srcStr, int srcStrLength, Token **srcTokenArrPtr, int *numTokens){
    int numPossibleTokens = 4;  //srcTokenArr can store 4 tokens without expanding
    *numTokens = 0;
    *srcTokenArrPtr = malloc(numPossibleTokens*sizeof(Token));
    if(!srcTokenArrPtr) fprintf(stderr, "[ERR] CAN'T ALLOCATE INITIAL TOKENS\n");

    Token *t = token_append(srcTokenArrPtr, numTokens, &numPossibleTokens); t->type = identifier; t->contents = "first";
    
    int curIndex = 0;   //index of begining of current token
    int curOffset = 0;  //offset of index to read srcStr from curIndex to curOffset
    for(; curOffset < srcStrLength; ++curOffset){
        
    }

    printf("[INFO] Finished parsing string of %i tokens, and %i possible tokens\n", *numTokens, numPossibleTokens);
}

/**
 * @brief Parse a file into tokens
 * @param srcFile input file to parse
 * @param srcTokenArr pointer to NULL
 * @param srcTokenArr pointer to int for writing the total number of processed tokens
 */
void token_tokenize_file(const char *srcFilepath, Token **srcTokenArr, int *numTokens){
    FILE *srcFile;
    srcFile = fopen(srcFilepath, "r");  //open source file for reading

    if(srcFile != NULL){
        fseek(srcFile, 0, SEEK_END);    //set cur to end of file to read file size below
        int srcSize = ftell(srcFile);   //set srcSize to number of characters in file
        char* srcStr = malloc(srcSize); //store source string on heap

        fseek(srcFile, 0, SEEK_SET);    
        fread(srcStr, 1, srcSize, srcFile); //read source file contents into source string

        token_tokenize_string(srcStr, srcSize, srcTokenArr, numTokens); //feed source string into tokenization function
        free(srcStr);
    }else{
        fprintf(stderr, "[ERR] CAN'T OPEN FILE: %s\n", srcFilepath);  //print error if file cant be opened
    }

    fclose(srcFile);
}

/**
 * @brief print contents of tokens to stdout.
 * PRECONDITION: tokens up to numTokens is initialized with tokens
 * @param tokens pointer to begnining of tokens in memory
 * @param numTokens number of consecutive tokens
 */
void token_arr_print(Token *tokens, int numTokens){
    const char *typeNames[] = {"Identifier", "Keyword", "Operator", "numLiteral", "strLiteral"};
    printf("NUM TOKENS: %i:\n", numTokens);
    for(int i = 0; i < numTokens; ++i)
        printf("Type: %s    Contents: %s\n", typeNames[tokens[i].type], tokens[i].contents);
}

int main(){
    Token *srcTokenArr = NULL;

    int numTokens;
    token_tokenize_file("./res/test.vl", &srcTokenArr, &numTokens);

    token_arr_print(srcTokenArr, numTokens);

    free(srcTokenArr);

    return 0;
}