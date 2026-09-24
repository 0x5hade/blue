#include <bits/stdc++.h>
#include <string>
#include "lexer.h"

using namespace std;

int main() {
  string src((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());

  Lexer lexer;
  
  // Get tokens tokens
  vector<Token> tokens = lexer.scanTokens(src);

  
  return 0;
}
