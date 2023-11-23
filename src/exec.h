#pragma once

#include <map>
#include <stack>
#include <vector>
#include "stream.h"

class Executor: public Stream {
public:
    Executor(Stream* istr): input{istr} {}
    ~Executor() { clear(); }

    void read();
    Executor& operator>>(Token*& token) { token = it<vec.size()? vec[it++]: nullptr; _eof=token==nullptr; return *this; }
    size_t size() { return vec.size(); }
    void clear() { it=0; for (auto el: vec) delete el; while (!stack.empty()) stack.pop(); vec.clear(); }
    Token* result() { return vec.empty()? nullptr: *vec.rbegin(); }
private:
    Stream* input;
    std::stack<Token*> stack;
    std::vector<Token*> vec;
    std::map<std::string, MemberToken*> var_map;

    void exec_internal(Token* A, Token* B, char operation);
    void do_exec(char operation, bool unar);

    template<typename T> void exec_unar(const T& A, char oper) {
    
        if (oper=='-') { vec.push_back(new T(-A)); stack.push(*vec.rbegin()); }                           
    }

    template<typename Ta, typename Tb, typename Tr> void exec_dbl(Token* A, Token* B, char operation) {

        switch (operation) {
            case '+': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) + *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
            case '-': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) - *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
            case '*': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) * *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
            case '/': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) / *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
        }        
    }

    void assign(Token* A, Token* B) { var_map[dynamic_cast<Variable*>(A)->str()]= (MemberToken*) B; }

    template<typename Ta, typename Tb, typename Tr> void exec_vec_spec1(Token* A, Token* B, char operation) {

        switch (operation) {
            case '+': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) + *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
            case '-': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) - *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
        }        
    }

    template<typename Ta, typename Tb, typename Tr> void exec_vec_spec2(Token* A, Token* B, char operation) {

        switch (operation) {
            case '*': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) * *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
            case '/': vec.push_back(new Tr(*(dynamic_cast<Ta*>(A)) / *(dynamic_cast<Tb*>(B)))); stack.push(*vec.rbegin()); break;
        }        
    }
};