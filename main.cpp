#include <bits/stdc++.h>
#include <string>
#include "lexer.h"
#include "parser.h"

using namespace std;

int main() {
  string src((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

  Lexer lexer;
  
  vector<Token> tokens = lexer.scanTokens(src);

  Parser parser(tokens);

  auto ast = parser.generate_ast();
  parser.traverse(ast);


  
  return 0;
}

#include "parser.cpp"
#include "lexer.cpp"
