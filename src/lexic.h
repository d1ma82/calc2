/*
        Lexical analysis

        Create token stream from user input stream
*/

#pragma once
#include <map>
#include <vector>
#include "stream.h"
#include "token.h"

class InvalidToken: public std::exception {};
class Quit: public std::exception {};

class TokenStream: public Stream {
public:
    TokenStream(std::istream& istr): input{istr} { input.exceptions(std::ios_base::eofbit); }
    ~TokenStream() { clear(); }

    void read();
    size_t size() { return tokens.size(); }
    void clear() {  
        for (auto el: tokens) 
            if (el->type >= T_PLUS && el->type <= T_EOEQ) continue; else delete el;
        tokens.clear();
        it=0; 
    }
    TokenStream& operator>>(Token*& token) { 
        
        token= it<tokens.size()? tokens[it++]: nullptr; 
        _eof=token==nullptr;
        return *this; 
    };
private:
    Type prev;
    std::istream& input;
    const std::map<char, OperatorToken> operators {
        {'+', Plus()}, {'-', Minus()}, {'u', Minus(true)}, 
        {'*', Multiply()}, {'/', Division()}, {'=', Assign()}
    };
    OpenParenthesis op;
    CloseParenthesis cp;
    EndEquation ee;

    std::vector<Token*> tokens;                 // Tokens creation place
};