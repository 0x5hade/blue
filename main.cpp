#include <bits/stdc++.h>
using namespace std;

// Lex source -> token stream. Output "<TYPE> <LEX> <LITERAL>" + final "EOF  null".

static const unordered_set<string> KEYWORDS = {
    "and","class","else","false","for","fun","if","nil","or","print","return",
    "super","this","true","var","while"
};
static const unordered_map<char, string> SINGLES = {
    {'(',"LEFT_PAREN"}, {')',"RIGHT_PAREN"}, {'{',"LEFT_BRACE"}, {'}',"RIGHT_BRACE"},
    {',',"COMMA"}, {'.',"DOT"}, {'-',"MINUS"}, {'+',"PLUS"},
    {';',"SEMICOLON"}, {'*',"STAR"}
};

int main() {
    string src((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    size_t i = 0; int line = 1;
    while (i < src.size()) {
        char c = src[i];
        if (c == '\n') { line++; i++; continue; }
        if (c == ' ' || c == '\r' || c == '\t') { i++; continue; }
        auto it = SINGLES.find(c);
        if (it != SINGLES.end()) {
            cout << it->second << " " << c << " null\n"; i++; continue;
        }
        // TODO: two-char ops (!=, ==, <=, >=) and BANG/EQUAL/LESS/GREATER variants.
        // TODO: '/' may be a SLASH or start a // comment.
        // TODO: '"' starts a STRING literal (track newlines, error if unterminated).
        // TODO: digit -> NUMBER (with optional fraction).
        // TODO: alpha/_ -> IDENTIFIER, lookup in KEYWORDS.
        i++;
    }
    cout << "EOF  null\n";
    return 0;
}
