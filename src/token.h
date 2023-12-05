#pragma once

#include <vector>
#include <sstream>
#include "log.h"

class VecrtorSize: public std::exception {};

enum Type {T_UNDEF, T_CONST, T_VECTOR, T_VAR, T_PLUS, T_MINUS, T_PRODUCT, T_DIV, T_ASSIGN, T_OP, T_CP, T_EOEQ};

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
    
    const char* str() { return snprintf(buf, sizeof(double)+1, "%f%c", value, '\0')<0? "NA": buf; }

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

class Vector: public MemberToken, public Member<std::vector<double>> {
public:
    Vector(const std::vector<double>& val) { type=T_VECTOR; value=std::move(val); }
    Vector(const Vector& vec):Vector(vec.value) {}
    ~Vector() {}

    const char* str() { std::stringstream buf; buf<<'{'; for (size_t i=0; i<value.size(); i++) buf<<value[i]<<(i==value.size()-1? '}': ','); buf_str=buf.str(); return buf_str.c_str(); }

    friend Vector operator+(const Vector& A, const Vector& B);
    friend Vector operator+(Const A, const Vector& B);
    friend Vector operator+(const Vector& A, Const B) { return B+A; }
    friend Vector operator-(const Vector& A) { std::vector<double> ret(A.value.size()); for (size_t i=0; i<A.value.size(); i++) ret[i]=-A.value[i]; return ret; }
    friend Vector operator-(const Vector& A, const Vector& B);
    friend Vector operator-(Const A, const Vector& B);
    friend Vector operator-(const Vector& A, Const B);
    friend Const operator*(const Vector& A, const Vector& B);
    friend Vector operator*(Const A, const Vector& B) ;
    friend Vector operator*(const Vector& A, Const B) { return B*A; }
    friend Const operator/(const Vector& A, const Vector& B) { return {0.0/0.0}; }
    friend Vector operator/(Const A, const Vector& B);
    friend Vector operator/(const Vector& A, Const B);
private:
    std::string buf_str;
};

class Plus: public OperatorToken {
public:
    Plus() { type=T_PLUS; symbol='+'; }
};

class Minus: public OperatorToken {
public:
    Minus() { type= T_MINUS; symbol='-'; }
    Minus(bool unar):Minus() { this->unar=unar; }
};

class Multiply: public OperatorToken {
public:
    Multiply() { type=T_PRODUCT; symbol='*'; }
};

class Division: public OperatorToken {
public:
    Division() { type=T_DIV; symbol='/'; }
};

class Assign: public OperatorToken {
public:
    Assign() { type=T_ASSIGN; symbol='=';}
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