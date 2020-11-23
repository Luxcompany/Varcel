#include <stdio.h>
#include "pipeline/Lexer.hpp"
#include "log.hpp"
#include "types.hpp"

const std::string numStartChars = "1234567890";
const std::string numChars = numStartChars + ".";

const std::string identifierStartChars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_";
const std::string identifierChars = identifierStartChars + numStartChars;

const std::string newlineChars = "\n\r";
const std::string whitespaceChars = " \t\1\5" + newlineChars;

enum class LexemeType{
    iden,       //identifier
    op,         //operator
    num,        //number literal
    str,        //string literal
    whitespace,
    comment,
    unknown
};

static LexemeType checkFirstCharType(const char& c){
    if(c == '\"')
        return LexemeType::str;

    if(c == '/' && ((&c)[1] == '/' || (&c)[1] == '*'))
        return LexemeType::comment;

    if(whitespaceChars.find(c) != std::string::npos)
        return LexemeType::whitespace;
    
    if(numStartChars.find(c) != std::string::npos)
        return LexemeType::num;

    if(identifierStartChars.find(c) != std::string::npos)
        return LexemeType::iden;

    return LexemeType::unknown;
}

namespace Lexer{
    void parseFile(std::string srcFilepath, std::vector<Token>& srcTokens){
        FILE *srcFile;
        srcFile = fopen(srcFilepath.c_str(), "r");  //open source file for reading

        if(srcFile != NULL){
            fseek(srcFile, 0, SEEK_END);    //set cur to end of file to read file size below
            uint srcSize = ftell(srcFile)+1;   //set srcSize to number of characters in file
            char* srcStr = (char*) malloc(srcSize); //store source string on heap

            fseek(srcFile, 0, SEEK_SET);    
            fread(srcStr, 1, srcSize, srcFile); //read source file contents into source string
            srcStr[srcSize-1] = '\0'; //cap off with null terminator

            parseString(srcStr, srcTokens); //feed source string into tokenization function
            free(srcStr);
        }else{
            logE("CAN'T OPEN FILE: %s", srcFilepath.c_str());  //print error if file cant be opened
        }

        fclose(srcFile);
    }

    void parseString(std::string srcString, std::vector<Token>& srcTokens){
        uint srcStringIndex = 0;
        const uint srcStringLength = srcString.length();

        while(srcStringIndex < srcStringLength){
            switch(checkFirstCharType(srcString[srcStringIndex])){
                case LexemeType::iden:{
                    uint srcStringBeginIndex = srcStringIndex;
                    while(identifierChars.find(srcString[srcStringIndex]) != std::string::npos)
                        ++srcStringIndex;
                    srcTokens.push_back( Token(TokenType::iden, srcString.substr(srcStringBeginIndex, srcStringIndex - srcStringBeginIndex)) );
                }break;

                case LexemeType::op:

                break;

                case LexemeType::num:{
                    uint srcStringBeginIndex = srcStringIndex;
                    while(numChars.find(srcString[srcStringIndex]) != std::string::npos)
                        ++srcStringIndex;

                    srcTokens.push_back( Token(TokenType::num, srcString.substr(srcStringBeginIndex, srcStringIndex - srcStringBeginIndex)) );
                }break;

                case LexemeType::str:{
                    ++srcStringIndex;
                    uint srcStringBeginIndex = srcStringIndex;

                    bool endOfFileReached = false;
                    while(
                        !( (srcString[srcStringIndex] == '\"' && srcString[srcStringIndex-1] != '\\') || (endOfFileReached = srcString[srcStringIndex] == '\0') )
                    ){++srcStringIndex;}

                    if(endOfFileReached)
                        logE("String isn't capped off with a quote: %s", srcString.c_str() + srcStringBeginIndex);

                    srcTokens.push_back( Token(TokenType::str, srcString.substr(srcStringBeginIndex, srcStringIndex - srcStringBeginIndex)) );
                    ++srcStringIndex;
                }break;

                case LexemeType::whitespace:
                    ++srcStringIndex;
                break;

                case LexemeType::comment:
                    if(srcString[srcStringIndex+1] == '/')
                        while(newlineChars.find(srcString[++srcStringIndex]) == std::string::npos && srcStringIndex < srcStringLength);

                    if(srcString[srcStringIndex+1] == '*'){
                        srcStringIndex += 2;
                        while(!(srcString[srcStringIndex] == '*' && srcString[srcStringIndex+1] == '/') && srcStringIndex < srcStringLength){++srcStringIndex;}
                        srcStringIndex += 2;
                    }
                break;

                case LexemeType::unknown:
                    logW("Can't identify lexeme from char %i: %c", (int)srcString[srcStringIndex], srcString[srcStringIndex]);
                    ++srcStringIndex;
                break;
            }
        }
    }
}