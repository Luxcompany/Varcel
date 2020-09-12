#pragma once
#include <string>
#include <vector>
#include "pipeline/Token.hpp"

namespace Lexer{
    void parseFile(std::string srcFilepath, std::vector<Token>& srcTokens);
    void parseString(std::string srcString, std::vector<Token>& srcTokens);
}