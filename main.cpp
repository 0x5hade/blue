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

  while (!parser.end()) {
    auto ast = parser.generate_ast();
    parser.traverse(ast);
    cout << '\n';
  }



  return 0;
}

#ifndef LOCAL_DEV
#include "parser.cpp"
#include "lexer.cpp"
#endif
