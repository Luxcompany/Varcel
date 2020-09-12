#include <iostream>
#include <vector>
#include "pipeline/Token.hpp"
#include "pipeline/Lexer.hpp"

int main(){
    std::vector<Token> srcTokens;
    Lexer::parseFile("./res/test.vl", srcTokens);

    std::cout << Token::getTokenVectorContents(srcTokens);
}