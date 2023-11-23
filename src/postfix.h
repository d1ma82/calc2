#pragma once

#include <stack>
#include <vector>
#include "stream.h"

class Postfix: public Stream {
public:
    Postfix(Stream* istr): input{istr}  {}
    ~Postfix() {};

    void read();
    size_t size() { return vec.size(); }
    void clear() { it=0; while (!stack.empty()) stack.pop(); vec.clear(); }
    Postfix& operator>>(Token*& token) { token = it<vec.size()? vec[it++]: nullptr; _eof=token==nullptr; return *this; };

private:
    Stream* input;
    std::stack<Token*> stack;            // for symbols  
    std::vector<Token*> vec;           // for constants

    void pop_until_empty();
    void pop_if_lower(int priority);        //  pop from stack 
    void close_parenthesis();
};