#include <stdio.h>
#include <stdlib.h>

/*
IM OKAY WITH HARDCODING LEXEMES - THEY SHOULDN'T BE CHANGED ANYWAYS
*/
const char seperators[] = " \n\t()[],";

const char operators[] =
"+ += ++ - -= -- * *= ** / /= % %= = == "
"> >= >> < <= << & && | || ^ ^^ ~ ! != "
".. @ ? :";

const char keywords[] =
"arr sizeof const mut freeze uses hidden "
"strict istype alias real int lambda amb string ";

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
    const char *value;
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
 * @brief Parse a string into a single token
 * @param str string to parse
 * @returns pointer to a token or NULL if token can't be parsed
 */
static Token *token_parse(const char* srcStr, int curIndex, int curOffset){
    static Token t;
    //TODO write token parsing algorithm -- check srcStr from curIndex to curOffset among lexemes
    return NULL;
}

/**
 * @brief Parse a string into tokens
 * @param srcStr input string to parse
 * @param srcStrLength Length of input string (number of chars)
 * @param srcTokens pointer to memory in heap created via malloc
 * @param srcTokens pointer to int for writing the total number of processed tokens
 */
void tokenize_string(const char *srcStr, int srcStrLength, Token *srcTokens, int *numTokens){
    Token *curToken;
    int curIndex = 0;   //index of begining of current token
    int curOffset = 0;  //offset of index to read srcStr from curIndex to curOffset
    for(; curOffset < srcStrLength; ++curOffset){
        if(curToken = token_parse(srcStr, curIndex, curOffset)){
            //TODO  append curToken to srcTokens & update numTokens (maybe use numTokens to keep track)
            curIndex = curOffset;   //start scanning from end of last token
        }else{
            ++curOffset;
        }
    }
}

/**
 * @brief Parse a file into tokens
 * @param srcFile input file to parse
 * @param srcTokens pointer to memory in heap created via malloc
 * @param srcTokens pointer to int for writing the total number of processed tokens
 */
void tokenize_file(const char *srcFilepath, Token *srcTokens, int *numTokens){
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
        fprintf(stderr, "[ERR] CAN'T OPEN FILE: %s", srcFilepath);  //print error if file cant be opened
    }

    fclose(srcFile);
}

int main(){
    Token *srcTokens = malloc(1);
    int numTokens;
    tokenize_file("./res/test.vl", srcTokens, &numTokens);

    free(srcTokens);

    return 0;
}