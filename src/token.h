#pragma once

#include <sstream>
#include "log.h"

enum Type {T_CONST, T_VECTOR, T_VAR, T_OPER, T_OP, T_CP, T_EOEQ};

class Token {
public:
    virtual ~Token()=default;
    virtual const char* str()=0;
    Type type;
protected:
    Token() {}
};

template<typename T> class Member {
public:
    virtual ~Member() = default;
    T& get() {return value; }
    void set(T& val) { value=val; }
protected:
    Member() {}
    T value;
};

// Expression member
class MemberToken: public Token {
public:
    virtual ~MemberToken() = default;
protected:
    MemberToken() {}
};

class OperatorToken: public Token {
public:
    virtual ~OperatorToken() = default;
    char get() const    { return symbol; }
    void set_unar()     { unar=true; }
    bool is_unar() const { return unar; }
    const char* str() { snprintf(buf, 16, "%c%c%s%c", symbol, '\t', (unar? "unar":""), '\0'); return buf; }
protected:
    OperatorToken() { }
    char buf[16];
    char symbol;
    bool unar {false};
};

class Const: public MemberToken, public Member<double> {
public:
    Const(double val){ type=T_CONST; value=val; }
    Const (const Const& val):Const(val.value) {}
    
    const char* str() { snprintf(buf, sizeof(double)+1, "%f%c", value, '\0'); return buf; }

    friend Const operator+(Const A, Const B) { return {A.value+B.value}; }
    friend Const operator-(Const A, Const B) { return {A.value-B.value}; }
    friend Const operator-(Const A)          { return {-A.value}; }
    friend Const operator*(Const A, Const B) { return {A.value*B.value}; }
    friend Const operator/(Const A, Const B) { return {A.value/B.value}; }
private:
    char buf[sizeof(double)+1];
};

class Variable: public MemberToken {
public:
    Variable(const char* name):name{name} { type=T_VAR; }

    const char* str() { return name.c_str(); }
private:
    std::string name;
};

class Plus: public OperatorToken {
public:
    Plus() { type=T_OPER; symbol='+'; }
};

class Minus: public OperatorToken {
public:
    Minus() { type= T_OPER; symbol='-'; }
};

class Multiply: public OperatorToken {
public:
    Multiply() { type=T_OPER; symbol='*'; }
};

class Division: public OperatorToken {
public:
    Division() { type=T_OPER; symbol='/'; }
};

class Assign: public OperatorToken {
public:
    Assign() { type=T_OPER; symbol='=';}
};

class OpenParenthesis: public OperatorToken {
public:
    OpenParenthesis() { type=T_OP; symbol='('; }
};

class CloseParenthesis: public OperatorToken {
public:
    CloseParenthesis() { type=T_CP; symbol=')'; }
};

class EndEquation: public OperatorToken {
public:
    EndEquation() { type=T_EOEQ; symbol=';'; }
};


const char* type_to_str(Type type);