#pragma once

#include <string>

struct Token { 
  std::string type;
  std::string lexeme; 
  std::string literal; 
  int line;
};
