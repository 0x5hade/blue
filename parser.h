#pragma once
#include <bits/stdc++.h>
#include <initializer_list>
#include <memory>
#include "token.h"

using namespace std;

struct Expr { virtual ~Expr() = default; };
struct Lit  : Expr { string value; };
struct Unary: Expr { string op; unique_ptr<Expr> right; };
struct Binary: Expr { unique_ptr<Expr> left; string op; unique_ptr<Expr> right; };
struct Group: Expr { unique_ptr<Expr> expr; };

class Parser {
  private:
    vector<Token> tokens;
    set<unique_ptr<Expr>> visited;

    size_t i = 0;

    Token& peek();
    Token& advance();
    bool check(const string& t);
    bool match(initializer_list<string> types);

    unique_ptr<Expr> expression();
    unique_ptr<Expr> equality();
    unique_ptr<Expr> comparison();
    unique_ptr<Expr> term();
    unique_ptr<Expr> factor();
    unique_ptr<Expr> unary();
    unique_ptr<Expr> primary();

  public:
    Parser(const vector<Token> &tokens);
    unique_ptr<Expr> generate_ast();
    void traverse(const unique_ptr<Expr> &node);
};
