#include <bits/stdc++.h>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

using namespace std;

int main() {
  string src((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

  Lexer lexer;
  
  vector<Token> tokens = lexer.scanTokens(src);

  Parser parser(tokens);

  Interpreter interpreter;

  while (!parser.end()) {
    auto ast = parser.generate_ast();
    // interpreter.traverse(ast);
    // cout << '\n';
    std::variant<string, int, float, bool> value;
    VariantValue result = interpreter.evaluate(ast);
    interpreter.print(result);
  }



  return 0;
}

#ifndef LOCAL_DEV
#include "parser.cpp"
#include "lexer.cpp"
#include "interpreter.cpp"
#endif
