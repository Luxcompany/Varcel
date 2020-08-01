#include <stdio.h>
#include <stdlib.h>

/*
IM OKAY WITH HARDCODING LEXEMES - THEY SHOULDN'T BE CHANGED ANYWAYS
*/
const char whitespaceChars[] = " \n\t";

const char *seperators[] = {"(", ")", "[", "]", ","};

const char *operators[] = {
"+", "+=", "++", "-", "-=", "--", "*", "*=", "**", "/", "/=", "%", "%=", "=", "==",
">", ">=", ">>", "<", "<=", "<<", "&", "&&", "|", "||", "^", "^^", "~", "!", "!=",
"..", "@", "?", ":"
};

const char *keywords[] = {
"arr", "sizeof", "const", "mut", "freeze", "uses", "hidden",
"strict", "istype", "alias", "real", "int", "lambda", "amb",
"string"
};

typedef enum {
    nil = -1,
    identifier,
    keyword,
    operator,
    seperator,
    literal
} TokenType;

typedef struct {
    TokenType type;
    const char * contents;
} Token;

/**
 * @brief compare a subsection of str1 to all of str2 - used in tokenize_string
 */
static int str_contains(const char* str1, int str1Offset, int str1MaxChars, const char* str2, int treatSpaceAsNUL){
    for(int i = 0; str2[i] != '\0' && (str2[i] != ' ' || treatSpaceAsNUL); ++i)
        if(str1[str1Offset + i] != str2[i] || i > str1MaxChars || str1[str1Offset + i] == '\0')
            return 0;
    return 1;
}

/**
 * @brief Appends token to end of srcTokens; expands srcTokens if not big enough - used in tokenize_string
 * @param srcTokens pointer to array of tokens
 * @param numTokens pointer to int for updating number of tokens
 * @param numPossibleTokens pointer to int for updating size of array
 * @return pointer to first empty token in srcTokens, or NULL
 */
static Token *token_append(Token **srcTokens, int *numTokens, int *numPossibleTokens){
    if( (*numTokens)+1 > *numPossibleTokens ){
        //DOUBLE SIZE OF srcTokens
        int newSize = (*numPossibleTokens)*2;
        Token *newSrcTokens = realloc(*srcTokens, newSize*sizeof(Token));
        //ADD ONE TO SIZE OF srcTokens IF DOUBLING FAILS
        if(!newSrcTokens){
            newSize = (*numPossibleTokens) + 1;
            newSrcTokens = realloc(*srcTokens, newSize*sizeof(Token));

            if(!newSrcTokens){
                fprintf(stderr, "[ERR] CAN'T EXPAND SRC TOKENS TO SIZE OF %i TOKENS\n", *numPossibleTokens);
                return NULL;
            }
        }

        printf("[INFO] Expanded numPossibleTokens to %i\n", newSize);

        *numPossibleTokens = newSize;
        *srcTokens = newSrcTokens;
    }

    //Return pointer to first empty token in srcTokens, and increment token counter
    ++(*numTokens);
    return &((*srcTokens)[(*numTokens)-1]);
}

/**
 * @brief Parse a string into tokens
 * @param srcStr input string to parse
 * @param srcStrLength Length of input string (number of chars)
 * @param srcTokens pointer to NULL
 * @param srcTokens pointer to int for writing the total number of processed tokens
 */
void tokenize_string(const char *srcStr, int srcStrLength, Token **srcTokens, int *numTokens){
    int numPossibleTokens = 4;  //srcTokens can store 4 tokens without expanding
    *numTokens = 0;
    *srcTokens = malloc(numPossibleTokens*sizeof(Token));
    if(!srcTokens) fprintf(stderr, "[ERR] CAN'T ALLOCATE INITIAL TOKENS\n");

    Token *t = token_append(srcTokens, numTokens, &numPossibleTokens); t->type = nil; t->contents = "first";
    
    int curIndex = 0;   //index of begining of current token
    int curOffset = 0;  //offset of index to read srcStr from curIndex to curOffset
    for(; curOffset < srcStrLength; ++curOffset){

    }

    printf("[INFO] Finished parsing string of %i tokens, and %i possible tokens\n", *numTokens, numPossibleTokens);
}

/**
 * @brief Parse a file into tokens
 * @param srcFile input file to parse
 * @param srcTokens pointer to NULL
 * @param srcTokens pointer to int for writing the total number of processed tokens
 */
void tokenize_file(const char *srcFilepath, Token **srcTokens, int *numTokens){
    FILE *srcFile;
    srcFile = fopen(srcFilepath, "r");  //open source file for reading

    if(srcFile != NULL){
        fseek(srcFile, 0, SEEK_END);    //set cur to end of file to read file size below
        int srcSize = ftell(srcFile);   //set srcSize to number of characters in file
        char* srcStr = malloc(srcSize); //store source string on heap

        fseek(srcFile, 0, SEEK_SET);    
        fread(srcStr, 1, srcSize, srcFile); //read source file contents into source string

        tokenize_string(srcStr, srcSize, srcTokens, numTokens); //feed source string into tokenization function
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
    const char *typeNames[] = {"Nil", "Identifier", "Keyword", "Operator", "Seperator", "Literal"};
    for(int i = 0; i < numTokens; ++i)
        printf("Type: %s    Contents: %s\n", typeNames[tokens[i].type+1], tokens[i].contents);
}

int main(){
    Token *srcTokens = NULL;
    int numTokens;
    tokenize_file("./res/test.vl", &srcTokens, &numTokens);

    printf("NUM TOKENS: %i\n", numTokens);
    token_arr_print(srcTokens, numTokens);

    free(srcTokens);

    return 0;
}