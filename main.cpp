#include <bits/stdc++.h>
#include <string>
#include <unordered_map>
using namespace std;

// Lex source -> token stream. Output "<TYPE> <LEX> <LITERAL>" + final "EOF
// null".

// static const unordered_set<char> DIGITS = {'0','1', '2', '3', '4', '5', '6', '7', '8', '9'};

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
// Remaining: !=, ==, >=, <=, / and //

int main() {
  string src((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
  size_t i = 0;
  int line = 1;
  stack<char> st;
  while (i < src.size()) {
    char c = src[i];
    bool two_op = false;

    if (c == '\n') {
      line++;
      i++;
      continue;
    }
    // If white-space or comment then skip.
    if (c == ' ' || c == '\r' || c == '\t') {
      i++;
      continue;
    }
    if (c == '/' && i + 1 < src.size() && src[i + 1] == '/') {
      line++;
      i += 2;

      // we need to get to the end of the line, so we go through each char till
      // we find \n;
      while (c != '\n' && i < src.size()) {
        i++;
        c = src[i];
      }
      continue;
    }

    // If !=, <=, >=, == then this is a two-char op.
    if ((c == '!' || c == '<' || c == '>' || c == '=') && i + 1 < src.size() &&
        src[i + 1] == '=') {
      two_op = true;
    }

    // Handle strings, Yet to continue as it is unfinished.
    // we start when we find a single quotation. Now, everything until we find the other one is part of the string, so we must go and continue till we find another quotation.
    if (c == '"') {
      i++;
      c = src[i];
      string str = "";

      // Add the string content;
      while (c != '"' && i < src.size()) {
        if (c == '\n') line++;
        str += c;
        i++;

        if (i >= src.size()) {
          cout << "Error! String literal unterminated\n";
          return 1;
        }
        c = src[i];

      } 
      i++;
      cout << "STRING" << " " << '"' << str << '"' << " " << str << '\n'; 
      continue;
    }

    if (two_op) {
      string twochr_op = "";
      twochr_op += c;
      twochr_op += src[i+1];
      // string twochr_op(2,  string("") + c + src[i + 1]);
      auto it = DOUBLES.find(twochr_op);
      if (it != DOUBLES.end()) {
        cout << it->second << " " << twochr_op << " null\n";
        i += 2;
        continue;
      }
    } else {
      auto it = SINGLES.find(c);
      if (it != SINGLES.end()) {
        cout << it->second << " " << c << " null\n";
        i++;
        continue;
      }
    }

    // abc
    if ((isalpha(c) || c == '_') && i < src.size()) {
      string keyword = "";
      while((isalpha(c) || c == '_') && i < src.size()) {
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

        cout << keyword_upper << " " << keyword << " " << "null\n";
        continue;
      } else {
        cout << "IDENTIFIER" << " " << keyword << " " << "null\n";
        continue;
      }
      i++;
    }

    // check if the curr char is a number 
    // 13562
    if (isdigit(c) && (i < src.size())) {
      // if it is, then we wanna know where the number ends. so we traverse it till the end
      string num = "";
      while((isdigit(c) || c == '.') && i < src.size()) {
        num += c;
        i++;
        c = src[i];
      }

      float numf = stof(num);
      if (floor(numf) == numf) {

      cout << "NUMBER" << " " << num << " " << numf << ".0" << '\n';
      } else {
      cout << "NUMBER" << " " << num << " " << numf << '\n';
      }
      continue;
    }

    // TODO: two-char ops (!=, ==, <=, >=) and BANG/EQUAL/LESS/GREATER variants.
    // TODO: '/' may be a SLASH or start a // comment.
    // TODO: '"' starts a STRING literal (track newlines, error if
    // unterminated).
    // TODO: digit -> NUMBER (with optional fraction).
    // TODO: alpha/_ -> IDENTIFIER, lookup in KEYWORDS.
    i++;
  }
  cout << "EOF  null\n";
  return 0;
}
