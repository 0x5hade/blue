#include "interpreter.h"
#include <memory>
#include <stdexcept>
#include <variant>

void Interpreter::traverse(const unique_ptr<Expr> &node) {
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

}

bool checkEqualEqual(VariantValue &left, VariantValue &right) {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum == rnum;
      }

      if (holds_alternative<bool>(left) && holds_alternative<bool>(right)) {
        bool lval = get<bool>(left);
        bool rval = get<bool>(right);
        return lval == rval;
      }


      if (holds_alternative<string>(left) && holds_alternative<string>(right)) {
        string lstr = get<string>(left);
        string rstr = get<string>(right);

        if (lstr.size() != rstr.size()) return false;

        for (int i = 0; i < lstr.size(); i++) {
          if (lstr[i] != rstr[i]) {
            return false;
          }
        }

        return true;
      }

      if (holds_alternative<monostate>(left) && holds_alternative<monostate>(right)) {
        return true;
      } 

      return false;

}

VariantValue Interpreter::evaluate(const unique_ptr<Expr> &node) {
  if (node == nullptr) {
    return monostate();
  }


  Expr* rnode = node.get();
  Binary *b = dynamic_cast<Binary*>(rnode);

  if (b != nullptr) {
    VariantValue right = evaluate(b->right);
    VariantValue left = evaluate(b->left);

    if (b->op == "+") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum + rnum;
      }

      if (holds_alternative<string>(left) && holds_alternative<string>(right)) {
        string lval = get<string>(left);
        string rval = get<string>(right);
        string result = "";
        result += lval; result += rval;
        return result;
      }
      throw runtime_error("Runtime Error: Can not add different types!");
    }


    if (b->op == "-") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum - rnum;
      }

      throw runtime_error("Runtime Error: Can not subtract different types!");
    }

    if (b->op == "*") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum * rnum;
      }
      throw runtime_error("Runtime Error: Can not multiply different types!");
    }

    if (b->op == "/") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);

        if (rnum == 0) {
          throw runtime_error("Runtime Error: Can not divide over zero!");
        }
        return lnum / rnum;
      }
      throw runtime_error("Runtime Error: Can not divide different types!");
    }

    if (b->op == ">") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum > rnum;
      }
      throw runtime_error("Runtime Error: Comparison must be between numbers!");
    }

    if (b->op == "<") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum < rnum;
      }
      throw runtime_error("Runtime Error: Comparison must be between numbers!");

    }

    if (b->op == "==") {
      bool result = checkEqualEqual(left, right);
      return result;
    }

    if (b->op == "!=") {
      bool result = checkEqualEqual(left, right);
      return !result;
    }

    if (b->op == ">=") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return lnum >= rnum;
      }

      throw runtime_error("Runtime Error: Can only compare numbers!");
    }
    if (b->op == "<=") {
      if (holds_alternative<float>(left) && holds_alternative<float>(right)) {
        float lnum = get<float>(left);
        float rnum = get<float>(right);
        return rnum >= lnum;
      }
      throw runtime_error("Runtime Error: Can only compare numbers!");
    }
  }

  Unary *u = dynamic_cast<Unary*>(rnode);

  if (u != nullptr) {
    VariantValue right = evaluate(u->right);

    if (u->op == "-") {
      if (holds_alternative<float>(right)) {
        float num = get<float>(right);
        return -num;
      }

      throw runtime_error("Runtime Error: Can not negate a non-float value!");
    }


    if (u->op == "!") {
      if (holds_alternative<bool>(right)) {
        bool val = get<bool>(right);
        return !val;
      }

      if (holds_alternative<monostate>(right)) {
        return true;
      } 

      return false;
    }
  }

  Lit *l = dynamic_cast<Lit*>(rnode);
  if (l != nullptr) {

    // Number
    if (l->type == "NUMBER") {
      return stof(l->value);
    }

    if (l->type == "STRING") {
      return l->value;
    }

    // Boolean
    if (l->type == "TRUE") {
      return true;
    }

    if (l->type == "FALSE") {
      return false;
    }

    if (l->type == "NIL") {
      return monostate();
    }
  }

  Group *g = dynamic_cast<Group*>(rnode);
  if (g != nullptr) {
    return evaluate(g->expr);
  }

  return monostate();
}

void Interpreter::print(VariantValue &val) {
  if (holds_alternative<float>(val)) {
    float num = get<float>(val);
    cout << num;
  }

  if (holds_alternative<string>(val)) {
    string str = get<string>(val);
    cout << str;
  }

  if (holds_alternative<bool>(val)) {
    bool bval = get<bool>(val);
    string result = bval ? "true" : "false";
    cout << result;
  }

  if (holds_alternative<monostate>(val)) {
    monostate nil = get<monostate>(val);
    cout << "nil";
  }
}
