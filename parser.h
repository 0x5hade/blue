#pragma once
#include <bits/stdc++.h>
#include <initializer_list>
#include <memory>
#include "token.h"

using namespace std;

class Parser {
  private:
    vector<Token> tokens;
    set<unique_ptr<Expr>> visited;

    size_t i = 0;

    Token& peek();
    Token& advance();
    Token& consume(const string& type, const string& message);
    bool check(const string& t);
    bool match(initializer_list<string> types);

    unique_ptr<Stmt> statement();
    unique_ptr<Stmt> printStmt();
    unique_ptr<Stmt> exprStmt();


    unique_ptr<Expr> expression();
    unique_ptr<Expr> equality();
    unique_ptr<Expr> comparison();
    unique_ptr<Expr> term();
    unique_ptr<Expr> factor();
    unique_ptr<Expr> unary();
    unique_ptr<Expr> primary();

  public:
    Parser(const vector<Token> &tokens);
    vector<unique_ptr<Stmt>> parse();
    bool end();
};
