#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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
"..", "@", "?", ":", "(", ")", "[", "]", ","
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

typedef struct {
    const char *operator;
    List subOperators;
} OperatorTreeNode;

/** @brief checks if str contains substr from begining rather than anywhere */
static uint str_contains(const char *str, const char *substr){
    for(uint i=0; substr[i] != '\0'; ++i)
        if(str[i] != substr[i])
            return 0;
    
    return 1;
}

/**
 * @brief inserts operator into operatorTree recursively
 * @param op operator to insert
 * @param ops list of trees to insert it into
 */
static void operator_tree_insert(const char *op, List ops){
    //for ops: check if op contains op at i
    for(uint i=0; i < list_size(ops); ++i){
        OperatorTreeNode parentOp = list_get(ops, i, OperatorTreeNode);
        if(str_contains(op, parentOp.operator)){
            operator_tree_insert(op, parentOp.subOperators);    //recurse on sub operators of parent op if above is true
            return;     //exit function as to not append to current node
        }
    }
    //The rest is only executed if the size of ops is 0 -- appends op to ops
    OperatorTreeNode *newOp = list_append(ops);
    newOp->operator = op;
    newOp->subOperators = list_create(OperatorTreeNode);
}

/** used in operator_tree_init */
static int special_chars_contains(char c){
    for(uint i=0; i < list_size(specialChars); ++i)
        if(list_get(specialChars, i, char) == c)
            return 1;

    return 0;
}

/** @brief computes operator tree and special char */
static void operator_tree_init(){
    operatorTree = list_create(OperatorTreeNode);
    specialChars = list_create(char);

    for(uint i=0; i < sizeof(operators)/sizeof(const char *); ++i){ //for everything in operators
        operator_tree_insert(operators[i], operatorTree);           //insert it into operatorTree where it belongs
        if(!special_chars_contains(operators[i][0])){
            char *specialChar = list_append(specialChars);   //Append it to special chars if it's only one char long
            *specialChar = operators[i][0];
        }
    }
}

/**  @brief frees operator tree and special chars */
static void operator_tree_destroy(List ops){
    for(uint i=0; i < list_size(ops); ++i){
        OperatorTreeNode parentOp = list_get(ops, i, OperatorTreeNode);
        operator_tree_destroy(parentOp.subOperators);
    }
    list_destroy(ops);
}

/** @brief prints contents of operator tree - lvl starts as 0 */
static void operator_tree_print(List ops, uint lvl){
    for(uint i=0; i < list_size(ops); ++i){
        OperatorTreeNode op = list_get(ops, i, OperatorTreeNode);

        if(lvl==0) putc('\n', stdout);
        for(uint j=0; j<lvl; ++j) putc('\t', stdout);
        printf("%s\n", op.operator);

        if(list_size(op.subOperators)>0)
            operator_tree_print(op.subOperators, lvl+1);
    }
}

/** @brief returns the first operator starting at input */
static const char *operator_identify(const char *input, List ops){
    for(uint i=0; i < list_size(ops); ++i){
        OperatorTreeNode op = list_get(ops, i, OperatorTreeNode);
        if(str_contains(input, op.operator)){
            if(list_size(op.subOperators) > 0){
                const char *subOp = operator_identify(input, op.subOperators);
                if(subOp == NULL)
                    return op.operator;
                else
                    return subOp;
            }else{
                return op.operator;
            }
        }
    }
    return NULL;
}

static int type_check_identifier (char c) { for(uint i = 0; i < sizeof(identifierChars); ++i) if(identifierChars[i]              == c) return 1; return 0; }
static int type_check_operator   (char c) { for(uint i = 0; i < list_size(specialChars); ++i) if(list_get(specialChars, i, char) == c) return 1; return 0; }
static int type_check_numLiteral (char c) { for(uint i = 0; i < sizeof(numberChars);     ++i) if(numberChars[i]                  == c) return 1; return 0; }
static int type_check_strLiteral (char c) { return c == '\"'; }

/** @brief Check type based on first char in token
 *  @returns token type
 */
static TokenType type_check_all(char c){
    //ordered for efficiency - less intensive checks are first
    if( type_check_strLiteral(c) ) return strLiteral;
    if( type_check_numLiteral(c) ) return numLiteral;
    if( type_check_operator(c)   ) return operator;
    if( type_check_identifier(c) ) return identifier;
    return unknown;
}

void lexer_parse_string(const char *srcStr, uint srcStrLength, List *srcTokenListPtr, uint *numTokens){
    if(!(*srcTokenListPtr)){logE("TOKEN LIST ISN'T INITIALIZED"); return;}
    if(!operatorTree) operator_tree_init();
    //operator_tree_print(operatorTree,0);
    
    TokenType curType = unknown;
    uint curLIndex = 0;            //current left index - index of begining of current token
    uint curRIndex = 0;            //current right index - index of end of current token
    uint curTokenLength = 0;       //number of srcStr chars to read from curLIndex to curRIndex
    uint endOfFile = 0;            //for checking if end of file has been reached
    for(; curRIndex <= srcStrLength; ++curRIndex){
        curTokenLength = curRIndex-curLIndex;
        endOfFile = curRIndex == srcStrLength;
        //printf("type %i\n", curType);
        switch(curType){
            case unknown:
                //TODO check for comments
                curType = type_check_all(srcStr[curRIndex]);
                curLIndex = curRIndex;
            break;

            case identifier:  //contained in it's own scope (the if statement)
                if( !(type_check_identifier(srcStr[curRIndex]) || type_check_numLiteral(srcStr[curRIndex])) || endOfFile){ //check if last char isnt identifier-char or number OR end of file
                    curType = unknown;
                    
                    Token *t = list_append(*srcTokenListPtr);
                    t->type = identifier;
                    t->contents = malloc(curTokenLength+1);         //malloc for text from src string and null termination char

                    for(uint i=0; i < curTokenLength; ++i)
                        (t->contents)[i] = srcStr[curLIndex+i];     //copy identifier name from srcstr to token contents
                    
                    (t->contents)[curTokenLength] = '\0';           //cap off string with null termination

                    --curRIndex;    //start next itteration at begining of following token
                    curLIndex = curRIndex;
                }
            break;

            case operator:{ //contained in it's own scope
                //Identify operator
                curType = unknown;
                const char *op = operator_identify(srcStr + curLIndex, operatorTree);
                if(op == NULL) logE("OPERATOR CAN'T BE IDENTIFIED HERE: %s", srcStr + curLIndex);
                curRIndex += strlen(op)-2; //-2 because -1 for null terminator and -1 for putting curRIndex at end of operator
                curLIndex = curRIndex;

                //Append it to token list
                Token *t = list_append(*srcTokenListPtr);
                t->type = operator;
                t->contents = (char *)op;   //discard const (janky)
            }break;

            case numLiteral:{ //contained in its own scope
                
            }break;

            //TODO maybe read string chars into list
            case strLiteral:  //contained in it's own scope (the if statement)
                if( (srcStr[curRIndex] == '\"' && srcStr[curRIndex-1] != '\\') || endOfFile){
                    if(endOfFile) logE("STRING END BEFORE END OF FILE: %s", srcStr + curLIndex);

                    curType = unknown;
                    ++curLIndex;    //ignore first quote - last one already ignored
                    curTokenLength = curRIndex - curLIndex;
                    
                    Token *t = list_append(*srcTokenListPtr);
                    t->type = strLiteral;
                    t->contents = list_create(char);         //malloc for text from src string and null termination char

                    for(uint i=0; i < curTokenLength; ++i)
                        *(char *)list_append(t->contents) = srcStr[curLIndex + i];     //copy identifier name from srcstr to token contents
                    
                    *(char *)list_append(t->contents) = '\0';           //cap off string with null termination

                    curLIndex = curRIndex;
                }
            break;
        }
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

void lexer_clean_up(uint numTokens, ...){
    va_list args;
    va_start(args, numTokens);

    for(uint n=0; n < numTokens; ++n){
        List *srcTokenList = va_arg(args, List *);
        for(uint i=0; i < list_size(*srcTokenList); ++i){
            Token t = list_get(*srcTokenList, i, Token);
            switch(t.type){
                case identifier: free(t.contents); break;
                case operator: /* DO NOTHING, OPERATORS ARE IN STATIC MEMORY */ break;
                case numLiteral: /* TBD */; break;
                case strLiteral: /* TBD */; break;
            }
        }
    }

    va_end(args);

    operator_tree_destroy(operatorTree);
}