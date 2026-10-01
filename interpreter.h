#pragma once
#include <bits/stdc++.h>
#include <initializer_list>
#include <memory>
#include "token.h"


using VariantValue = std::variant<std::string, float, bool, std::monostate>;

using namespace std;

class Interpreter {
  private:

  public:
    void traverse(const unique_ptr<Expr> &node);
    VariantValue evaluate(const unique_ptr<Expr> &node);
    void print(VariantValue &value);
    void execute(unique_ptr<Stmt> &stmt);
    void interpret(vector<unique_ptr<Stmt>> &statements);
};
