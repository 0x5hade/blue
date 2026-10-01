#include "parser.h"
#include <memory>
#include <stdexcept>
#include "bits/stdc++.h"

using namespace std;

Parser::Parser(const vector<Token> &tokens) {
  this->tokens = tokens;
  this->i = 0;
}

Token& Parser::peek() {
  return tokens[i];
}

Token& Parser::advance() {
  return tokens[i++];
}

bool Parser::check(const string& t) {
  return tokens[i].type == t;
}

bool Parser::match(initializer_list<string> types) {
  for (auto& t : types) {
    if (check(t)) {
      advance();
      return true; 
    }
  }
  return false;
}

Token& Parser::consume(const string& type, const string& message) {
  if (check(type)) {
    return advance();
  }

  throw runtime_error("[line " + to_string(peek().line) + "] Error: " + message);
}

vector<unique_ptr<Stmt>> Parser::parse() {
  vector<unique_ptr<Stmt>> statements;
  while (!this->end()) {
    auto s = statement();
    statements.push_back(std::move(s));
  }

  return statements;
}

unique_ptr<Stmt> Parser::statement() {
  if (match({"PRINT"})) {
    return printStmt();
  }
  return exprStmt();
}

unique_ptr<Stmt> Parser::printStmt() {
  unique_ptr<Expr> expr = expression();

  consume("SEMICOLON", "Expected ; at the end of statement.");
  
  auto p = make_unique<PrintStmt>();
  p->expr = std::move(expr);

  return p;
}

unique_ptr<Stmt> Parser::exprStmt() {
  unique_ptr<Expr> expr = expression();

  consume("SEMICOLON", "Expected ; at the end of statement.");

  auto p = make_unique<ExprStmt>();
  p->expr = std::move(expr);
  
  return p;
}

unique_ptr<Expr> Parser::expression() { return equality(); }

unique_ptr<Expr> Parser::equality() {
  auto left = comparison();
  while (match({"BANG_EQUAL","EQUAL_EQUAL"})) {
    auto b = make_unique<Binary>();
    b->left = std::move(left);
    // b->op = tokens[i-1].type;
    b->op = tokens[i-1].lexeme;
    b->right = comparison();

    // store the tree to return it afterwards.
    left = std::move(b);
  }
  return left;
}

unique_ptr<Expr> Parser::comparison() {
  auto left = term();
  while (match({"GREATER","GREATER_EQUAL", "LESS", "LESS_EQUAL"})) {
    auto b = make_unique<Binary>();
    b->left = std::move(left);
    // b->op = tokens[i-1].type;
    b->op = tokens[i-1].lexeme;
    b->right = term();

    // store the tree to return it afterwards.
    left = std::move(b);
  }
  return left;
}

unique_ptr<Expr> Parser::term() {
  auto left = factor();
  while (match({"MINUS", "PLUS"})) {
    auto b = make_unique<Binary>();
    b->left = std::move(left);
    // b->op = tokens[i-1].type;
    b->op = tokens[i-1].lexeme;
    b->right = factor();


    // store the tree to return it afterwards.
    left = std::move(b);
  }
  return left; 
}

unique_ptr<Expr> Parser::factor() {
  auto left = unary();
  while (match({"SLASH", "STAR"})) {
    auto b = make_unique<Binary>();
    b->left = std::move(left); 
    // b->op = tokens[i-1].type;
    b->op = tokens[i-1].lexeme;
    b->right = unary();

    // store the tree to return it afterwards.
    left = std::move(b);
  }
  return left;
}

unique_ptr<Expr> Parser::unary() {
  if (match({"BANG","MINUS"})) {
    auto u = make_unique<Unary>();
    // u->op = tokens[i-1].type; 
    u->op = tokens[i-1].lexeme; 
    u->right = unary();
    return u;
  }
  return primary();
}
unique_ptr<Expr> Parser::primary() {
  if (match({"NUMBER","STRING","TRUE","FALSE","NIL"})) {
    auto l = make_unique<Lit>();
    l->value = tokens[i-1].literal;
    l->type = tokens[i-1].type;
    return l;
  }

  if (match({"LEFT_PAREN"})) {
    auto g = make_unique<Group>();
    g->expr = expression();
    if (match({"RIGHT_PAREN"})) {
      return g;
    }
    throw runtime_error("Unclosed parenthesis.");
  }

  throw runtime_error("Expect expression.");
}

bool Parser::end() {
  return tokens[i].type == "EOF";
}
