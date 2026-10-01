#pragma once
#include "bits/stdc++.h"
#include <memory>

using namespace std;


struct Token { 
  std::string type;
  std::string lexeme; 
  std::string literal; 
  int line;
};

struct Expr { virtual ~Expr() = default; };
struct Lit  : Expr { string type; string value; };
struct Unary: Expr { string op; unique_ptr<Expr> right; };
struct Binary: Expr { unique_ptr<Expr> left; string op; unique_ptr<Expr> right; };
struct Group: Expr { unique_ptr<Expr> expr; };

struct Stmt { virtual ~Stmt() = default; };
struct ExprStmt : Stmt {  unique_ptr<Expr> expr; };
struct PrintStmt : Stmt { unique_ptr<Expr> expr; };
