#include "parser.h"
#include <memory>

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



unique_ptr<Expr> Parser::generate_ast() { return expression(); }

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
  // TODO: term ((">"|">="|"<"|"<=") term)*
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
  // TODO: factor (("-"|"+") factor)*
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
  // TODO: unary (("/"|"*") unary)*
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

void Parser::traverse(const unique_ptr<Expr> &node) {
  // visit -> mark visited, left, right
  if (node == nullptr) {
    return;
  }

  Expr* rnode = node.get();
  Binary *b = dynamic_cast<Binary*>(rnode);

  if (b != nullptr) {
    cout << "(" << b->op << " ";
    traverse(b->left);
    cout << " ";
    traverse(b->right);
    cout << ")";
    return;
  }

  Unary *u = dynamic_cast<Unary*>(rnode);

  if (u != nullptr) {
    cout << "(" << u->op << " ";
    traverse(u->right);
    cout << ")";
    
    return;
  }

  Lit *l = dynamic_cast<Lit*>(rnode);
  if (l != nullptr) {
    cout << l->value;
    return;
  }

  Group *g = dynamic_cast<Group*>(rnode);
  if (g != nullptr) {
    cout << "(group ";
    traverse(g->expr);
    cout << ")";
  }


  // vars: node, visited stack?
  // stop condition: when there is no more nodes to visit in the tree. if (node->left && node->right are visited?) return;
  // visit left, visit right.

}
