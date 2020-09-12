#include "pipeline/Token.hpp"

Token::Token(TokenType type, std::string contents) : type(type), contents(contents){}

static std::string tokenTypeToString(TokenType t){
    std::string typeNames[] = {"Identifier-", "Operator---", "NumLiteral-", "StrLiteral-", "Unknown----"};
    return typeNames[(int)t];
}

std::string Token::getTokenVectorContents(const std::vector<Token>& tokens){
    std::string contents;
    for(Token t : tokens)
        contents += tokenTypeToString(t.type) + ":   " + t.contents + "\n";
    return contents;
}