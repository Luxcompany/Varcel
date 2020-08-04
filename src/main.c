#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list.h>

/*
IM OKAY WITH HARDCODING LEXEMES - THEY SHOULDN'T BE CHANGED ANYWAYS
*/
const char whitespaceChars[] = " \n\t";

const char *operators[] = {
"+", "+=", "++", "-", "-=", "--", "*", "*=", "**", "/", "/=", "%", "%=", "=", "==",
">", ">=", ">>", "<", "<=", "<<", "&", "&&", "|", "||", "^", "^^", "~", "!", "!=",
".", "..", "@", "?", ":", "(", ")", "[", "]", ","
};

const char *operatorTree[] = {
{"+", "+=", "++"}, {"-", "-=", "--"}, {"*", "*=", "**"}, {"/", "/="}, {"%", "%="}, {"=", "=="},
{">", ">=", ">>"}, {"<", "<=", "<<"}, {"&", "&&"}, {"|", "||"}, {"^", "^^"}, {"~"}, {"!", "!="},
{".", ".."}, {"@"}, {"?"}, {":"}, {"("}, {")"}, {"["}, {"]"}, {","}
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
static int str_compare(const char *str1, int str1Offset, int charsToCompare, const char *str2){
    for(int i = 0; (str2[i] != '\0')&&(i < charsToCompare); ++i)
        if(str1[str1Offset + i] != str2[i] || str1[str1Offset + i] == '\0')
            return 0;
    return 1;
}

/**
 * @brief Appends token to end of srcTokenList; expands srcTokenList if not big enough - used in token_tokenize_string
 * @param srcTokenListPtr pointer to array of tokens
 * @param numTokens pointer to int for updating number of tokens
 * @param numPossibleTokens pointer to int for updating size of array
 * @return pointer to first empty token in srcTokenList, or NULL
 * 
static Token *token_append(Token **srcTokenListPtr, int *numTokens, int *numPossibleTokens){
    if( (*numTokens)+1 > *numPossibleTokens ){
        //DOUBLE SIZE OF srcTokenList
        int newSize = (*numPossibleTokens)*2;
        Token *newSrcTokens = realloc(*srcTokenListPtr, newSize*sizeof(Token));
        //ADD ONE TO SIZE OF srcTokenList IF DOUBLING FAILS
        if(!newSrcTokens){
            newSize = (*numPossibleTokens) + 1;
            newSrcTokens = realloc(*srcTokenListPtr, newSize*sizeof(Token));

            if(!newSrcTokens){
                fprintf(stderr, "[ERR] CAN'T EXPAND SRC TOKENS TO SIZE OF %i TOKENS\n", *numPossibleTokens);
                return NULL;
            }
        }

        printf("[INFO] Expanded numPossibleTokens to %i\n", newSize);

        *numPossibleTokens = newSize;
        *srcTokenListPtr = newSrcTokens;
    }

    //Return pointer to first empty token in srcTokenList, and increment token counter
    ++(*numTokens);
    return &((*srcTokenListPtr)[(*numTokens)-1]);
}
*/

/**
 * @brief Parse a string into tokens
 * @param srcStr input string to parse
 * @param srcStrLength Length of input string (number of chars)
 * @param srcTokenListPtr pointer to initialized list of tokens
 * @param numTokens pointer to int for writing the total number of processed tokens
 */
void token_tokenize_string(const char *srcStr, int srcStrLength, List *srcTokenListPtr, int *numTokens){
    //int numPossibleTokens = 4;  //srcTokenList can store 4 tokens without expanding
    //*numTokens = 0;
    //*srcTokenListPtr = malloc(numPossibleTokens*sizeof(Token));
    if(!(*srcTokenListPtr)) fprintf(stderr, "[ERR] TOKEN LIST ISN'T INITIALIZED\n");

    Token *t = list_append(srcTokenListPtr); t->type = identifier; t->contents = "first";
    
    int curIndex = 0;   //index of begining of current token
    int curOffset = 0;  //offset of index to read srcStr from curIndex to curOffset
    for(; curOffset < srcStrLength; ++curOffset){
        
    }

    *numTokens = list_size(*srcTokenListPtr);
    printf("[INFO] Finished parsing string of %i tokens with %i left over possible tokens\n", *numTokens, list_possible_size(*srcTokenListPtr));
}

/**
 * @brief Parse a file into tokens
 * @param srcFile input file to parse
 * @param srcTokenList pointer to initialized list of tokens
 * @param srcTokenList pointer to int for writing the total number of processed tokens
 */
void token_tokenize_file(const char *srcFilepath, List *srcTokenList, int *numTokens){
    FILE *srcFile;
    srcFile = fopen(srcFilepath, "r");  //open source file for reading

    if(srcFile != NULL){
        fseek(srcFile, 0, SEEK_END);    //set cur to end of file to read file size below
        int srcSize = ftell(srcFile);   //set srcSize to number of characters in file
        char* srcStr = malloc(srcSize); //store source string on heap

        fseek(srcFile, 0, SEEK_SET);    
        fread(srcStr, 1, srcSize, srcFile); //read source file contents into source string

        token_tokenize_string(srcStr, srcSize, srcTokenList, numTokens); //feed source string into tokenization function
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
void token_list_print(List tokens, int numTokens){
    const char *typeNames[] = {"Identifier", "Keyword", "Operator", "numLiteral", "strLiteral"};
    printf("NUM TOKENS: %i:\n", numTokens);
    for(int i = 0; i < numTokens; ++i)
        printf("Type: %s    Contents: %s\n", typeNames[ ((Token *)list_get(tokens, i)) -> type ], ((Token *)list_get(tokens, i)) -> contents);
}

int main(){
    List srcTokenList = list_create(sizeof(Token));
    int numTokens;
    token_tokenize_file("./res/test.vl", &srcTokenList, &numTokens);

    token_list_print(srcTokenList, numTokens);

    list_destroy(srcTokenList);

    return 0;
}