#pragma once
#include <bits/stdc++.h>
#include <initializer_list>
#include <memory>
#include "token.h"

// #define VariantValue variant<string, float, bool, monostate>

using VariantValue = std::variant<std::string, float, bool, std::monostate>;

using namespace std;

class Interpreter {
  private:

  public:
    void traverse(const unique_ptr<Expr> &node);
    VariantValue evaluate(const unique_ptr<Expr> &node);
    void print(VariantValue &value);

};
