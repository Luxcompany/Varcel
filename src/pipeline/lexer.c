#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <log.h>
#include <pipeline/token.h>
#include <pipeline/lexer.h>

static const char whitespaceChars[] = " \n\t";
static const char numberChars[] = "1234567890";
static const char identifierChars[] =
"qwertyuiopasdfghjklzxcvbnm"
"QWERTYUIOPASDFGHJKLZXCVBNM"
"_";

static const char *operators[] = {
"+", "+=", "++", "-", "-=", "--", "*", "*=", "**", "/", "/=", "%", "%=", "=", "==",
">", ">=", ">>", "<", "<=", "<<", "&", "&&", "|", "||", "^", "^^", "~", "!", "!=",
".", "..", "@", "?", ":", "(", ")", "[", "]", ","
};

//Generated at runtime
static List specialChars = NULL;
static List operatorTree = NULL;

//moving this to semantic analyzer
/*const char *keywords[] = {
"arr", "sizeof", "const", "mut", "freeze", "uses", "hidden",
"strict", "istype", "alias", "real", "int", "lambda", "amb",
"string", "map"
};*/

/**
 * @brief computes operator tree and special chars
 */
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

/**
 * @brief frees operator tree and special chars
 */
static void operator_tree_destroy(){
    for(size_t i=0; i<list_size(operatorTree); ++i){
        List *opGroup = (List *)list_get(operatorTree, i);
        list_destroy(*opGroup);
    }
    list_destroy(operatorTree);
    list_destroy(specialChars);
}

/**
 * @brief prints contents of operator tree
 */
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

/**
 * @brief compare a subsection of str1 to all of str2 - used in lexer_parse_string
 */
static uint str_compare(const char *str1, uint str1Offset, uint numCharsToCompare, const char *str2){
    for(uint i = 0; (str2[i] != '\0')&&(i < numCharsToCompare); ++i)
        if(str1[str1Offset + i] != str2[i] || str1[str1Offset + i] == '\0')
            return 0;
    return 1;
}

void lexer_parse_string(const char *srcStr, uint srcStrLength, List *srcTokenListPtr, uint *numTokens){
    if(!(*srcTokenListPtr)){logE("TOKEN LIST ISN'T INITIALIZED"); return;}
    if(!operatorTree) operator_tree_init();
    //operator_tree_print();

    Token *t = list_append(srcTokenListPtr); t->type = identifier; t->contents = "first";
    
    uint curIndex = 0;   //index of begining of current token
    uint curOffset = 0;  //offset of index to read srcStr from curIndex to curOffset
    for(; curOffset < srcStrLength; ++curOffset){
        
    }

    *numTokens = list_size(*srcTokenListPtr);
    logI("Finished parsing string of %i tokens with %i left over possible tokens", *numTokens, list_possible_size(*srcTokenListPtr));
}

void lexer_parse_file(const char *srcFilepath, List *srcTokenList, uint *numTokens){
    FILE *srcFile;
    srcFile = fopen(srcFilepath, "r");  //open source file for reading

    if(srcFile != NULL){
        fseek(srcFile, 0, SEEK_END);    //set cur to end of file to read file size below
        uint srcSize = ftell(srcFile);   //set srcSize to number of characters in file
        char* srcStr = malloc(srcSize); //store source string on heap

        fseek(srcFile, 0, SEEK_SET);    
        fread(srcStr, 1, srcSize, srcFile); //read source file contents into source string

        lexer_parse_string(srcStr, srcSize, srcTokenList, numTokens); //feed source string into tokenization function
        free(srcStr);
    }else{
        logE("CAN'T OPEN FILE: %s", srcFilepath);  //print error if file cant be opened
    }

    fclose(srcFile);
}

void lexer_clean_up(){
    operator_tree_destroy();
}