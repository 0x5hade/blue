#include "lexer.h"
#include <math.h>
#include <iostream>
#include <string>


static const unordered_set<string> KEYWORDS = {
    "and", "class", "else",   "false", "for",  "fun",  "if",  "nil",
    "or",  "print", "return", "super", "this", "true", "var", "while"};

static const unordered_map<char, string> SINGLES = {
    {'(', "LEFT_PAREN"},  {')', "RIGHT_PAREN"}, {'{', "LEFT_BRACE"},
    {'}', "RIGHT_BRACE"}, {',', "COMMA"},       {'.', "DOT"},
    {'-', "MINUS"},       {'+', "PLUS"},        {';', "SEMICOLON"},
    {'*', "STAR"},        {'/', "SLASH"}, {'=', "EQUAL"}, {'!', "BANG"}, {'<', "LESS"}, {'>', "GREATER"}};

static const unordered_map<string, string> DOUBLES = {{"!=", "BANG_EQUAL"},
                                                      {"==", "EQUAL_EQUAL"},
                                                      {"<=", "LESS_EQUAL"},
                                                      {">=", "GREATER_EQUAL"}};


void Lexer::addToken(string type, string lexeme, string literal) {
  tokens.push_back({type, lexeme, literal, line});
};

std::vector<Token> Lexer::scanTokens(const string &src) {
  // Initialize scanner values first
  this->src = src;
  this->i = 0;
  this->line = 1;
  this->tokens.clear();

  // The main scanner loop
  while (i < src.size()) {
    char c = src[i];
    bool two_op = false;

    // Handling new-line characters
    if (c == '\n') {
      line++;
      i++;
      continue;
    }

    // Skipping white spaces.
    if (c == ' ' || c == '\r' || c == '\t') {
      i++;
      continue;
    }

    // Handling comments
    if (c == '/' && i + 1 < src.size() && (src[i + 1] == '/' || src[i+1] == '*')) {
      char cnext = src[i+1];
      i += 2;
      // one-line comments
      if (cnext == '/') {
        line++;

        // we need to get to the end of the line, so we go through each char till we find \n;
        while (c != '\n' && i < src.size()) {
          i++;
          c = src[i];
        }
        continue;

        // multi-line comments
      } else if (cnext == '*') {
        if (c == '\n')
          line++;

        while (i+1 < src.size() && (!(c == '*' && src[i+1] == '/'))) {
          i++;
          c = src[i];
        }
        i += 2;
        continue;
      }
    }

    // Check if two-char op. If !=, <=, >=, == then this is a two-char op.
    if ((c == '!' || c == '<' || c == '>' || c == '=') && i + 1 < src.size() &&
        src[i + 1] == '=') {
      two_op = true;
    }

    // Handling strings
    if (c == '"') {
      i++;
      c = src[i];
      string str_lexeme = "";
      string str_literal = "";

      // Add the string content;
      while (c != '"' && i < src.size()) {
        if (c == '\\') {
          char cnext = src[i+1];
          str_lexeme += '\\';
          str_lexeme  += cnext;
          i += 2;
          switch (cnext) {
            case 'n':
              str_literal += '\n';
              line++;
              break;
            case 't':
              str_literal += '\t';
              break;
            case '\\':
              str_literal += '\\';
              break;
            case '"':
              str_literal += '\"';
              break;
          }
        } else {
          str_lexeme += c;
          str_literal += c;
          i++;
        }
        if (i >= src.size()) {
          cerr << "[line " << line << "]" << " Error: unterminated string.\n";
          continue;
        }
        c = src[i];
      } 
      i++;
      // cout << "STRING" << " " << '"' << str_lexeme << '"' << " " << str_literal << '\n'; 
      addToken("STRING", str_lexeme, str_literal);
      continue;
    }

    // Handling operations (two-char operations and one-char operations)
    if (two_op) {
      string twochr_op = "";
      twochr_op += c;
      twochr_op += src[i+1];
      auto it = DOUBLES.find(twochr_op);
      if (it != DOUBLES.end()) {
        // cout << it->second << " " << twochr_op << " null\n";
        addToken(it->second, twochr_op, "null");
        i += 2;
        continue;
      }
    } else {
      auto it = SINGLES.find(c);
      if (it != SINGLES.end()) {
        // cout << it->second << " " << c << " null\n";
        string op_lexeme = "";
        op_lexeme += c;
        addToken(it->second, op_lexeme, "null");
        i++;
        continue;
      }
    }

    // Handling keywords & Identifiers
    if ((isalpha(c) || c == '_') && i < src.size()) {
      string keyword = "";
      while((isalpha(c) || c == '_' || isdigit(c)) && i < src.size()) {
        keyword += c;
        i++;
        c = src[i];
      }

      auto it = KEYWORDS.find(keyword);
      if (it != KEYWORDS.end()) {
        string keyword_upper = "";
        for (int i = 0; i < keyword.size(); i++) {
          keyword_upper += toupper(keyword[i]);
        }

        // cout << keyword_upper << " " << keyword << " " << "null\n";
        addToken(keyword_upper, keyword, "null");
        continue;
      } else {
        // cout << "IDENTIFIER" << " " << keyword << " " << "null\n";
        addToken("IDENTIFIER", keyword, "null");
        continue;
      }
      i++;
    }

    // Handling numbers 
    if (isdigit(c) && (i < src.size())) {
      // if it is, then we wanna know where the number ends. so we traverse it till the end
      string num = "";
      bool has_dot = false;
      while((isdigit(c) || c == '.') && i < src.size()) { 
        if (c == '.' && (has_dot || (!isdigit(src[i+1])))) {
          break;
        }

        if (c == '.') has_dot = true;

        num += c;
        i++;
        c = src[i];
      }

      float numf = stof(num);
      string num_literal = to_string(numf);
      // Check whether number is integer or float.
      if (floor(numf) == numf) {
        num_literal += ".0";

        // cout << "NUMBER" << " " << num << " " << numd << '\n';
        addToken("NUMBER", num, num_literal);
      } else {
        // cout << "NUMBER" << " " << num << " " << numf << '\n';
        addToken("NUMBER", num, num_literal);
      }
      continue;
    } 

    // Otherwise, handle as an unexpected character.
    cerr << "[line " << line << "]" << " Error: Unexpected character: " << c << '\n';

    i++;
  }
  // cout << "EOF  null\n";
  addToken("EOF", "", "null");

  return tokens;
};
