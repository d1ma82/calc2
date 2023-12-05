/*
        Synax analysis by compliance table
*/

#pragma once

#include <queue>
#include <stack>
#include <vector>
#include "token.h"
#include "stream.h"

class SyntaxException: public std::exception {};
class ParenthesisException: public std::exception {};

enum Word{W_UDEFINED, W_CONST, W_VECTOR, W_VAR, W_END_EQUATION, W_PLUS, W_MINUS, W_PRODUCT, W_DIV,
          W_OPEN_PARENTH, W_CLOSE_PARENTH,  W_ASSIGN, W_E, W_P, W_K, COUNT};  

typedef  std::queue<Word> Rule;

class Synt {
public:
    Synt(Stream* stream): input{stream} { begin.push(W_E); init_table(); }
    ~Synt() {}
    void read();
private:
    Stream* input {nullptr};
    int cnt_op {0}, cnt_cp {0};         // count open close parenthesis
    Rule begin;                         // Initial position of rule stack, must be W_E
    std::stack<Rule> rules;
    Rule table[3][Word::COUNT-3];  // rule table 3: W_E, W_P, W_K;

    void init_table();
    bool check(Token* token);
};