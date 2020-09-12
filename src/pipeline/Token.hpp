#pragma once
#include <string>
#include <vector>

enum class TokenType{
    iden,       //identifier
    op,         //operator
    num,        //number literal
    str,        //string literal
    unknown
};

class Token{
public:
    const TokenType type;
    std::string contents;
    Token(TokenType type, std::string contents);

    static std::string getTokenVectorContents(const std::vector<Token>& tokens);
};