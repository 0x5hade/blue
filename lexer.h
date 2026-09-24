#pragma once
#include "string"
#include "vector"
#include "token.h"
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Lexer {
  private:
    std::string src;
    size_t i = 0;
    int line = 1;
    std::vector<Token> tokens; 

    void addToken(std::string type, std::string lexeme, std::string literal);

  public:
    static const unordered_set<string> KEYWORDS;
    static const unordered_map<char, string> SINGLES;
    static const unordered_map<string, string> DOUBLES;
    std::vector<Token> scanTokens(const std::string &src);
};
