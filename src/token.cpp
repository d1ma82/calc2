#include "token.h"


const char* type_to_str(Type type) {

    switch (type) {
        
        case T_CONST:   return "const";
        case T_VECTOR:  return "vector";
        case T_VAR:     return "variable";
        case T_PLUS: case T_MINUS: case T_PRODUCT: case T_DIV: case T_ASSIGN:  return "operator";
        case T_OP:      return "open par";
        case T_CP:      return "close par";
        case T_EOEQ:    return "end of equation";   
        default: return "undefined"; 
    }
}

Vector operator+(const Vector& A, const Vector& B) {

    if(A.value.size()!=B.value.size()) throw VecrtorSize(); 
    std::vector<double> ret_vec;
    ret_vec.resize(A.value.size());
    std::vector<double>::const_iterator it1 = A.value.begin();
    std::vector<double>::const_iterator it2 = B.value.begin();

    for (auto& val: ret_vec) {

        val = *it1 + *it2;
        ++it1; ++it2;
    }
 
    return {ret_vec};
}

Vector operator+(Const A, const Vector& B) {

    std::vector<double> ret_vec;
    ret_vec.resize(B.value.size());
    std::vector<double>::const_iterator it = B.value.begin();

    for (auto& val: ret_vec) {

        val=A.get() + *it;
        ++it;
    }

    return {ret_vec};    
}

Vector operator-(const Vector& A, const Vector& B) {

    if(A.value.size()!=B.value.size()) throw VecrtorSize(); 
    std::vector<double> ret_vec;
    ret_vec.resize(A.value.size());
    std::vector<double>::const_iterator it1 = A.value.begin();
    std::vector<double>::const_iterator it2 = B.value.begin();

    for (auto& val: ret_vec) {

        val = *it1 - *it2;
        ++it1; ++it2;
    }
 
    return {ret_vec};
}

Vector operator-(Const A, const Vector& B) {

    std::vector<double> ret_vec;
    ret_vec.resize(B.value.size());
    std::vector<double>::const_iterator it = B.value.begin();

    for (auto& val: ret_vec) {

        val=A.get() - *it;
        ++it;
    }

    return {ret_vec};    
}

Vector operator-(const Vector& A, Const B) {

    std::vector<double> ret_vec;
    ret_vec.resize(A.value.size());
    std::vector<double>::const_iterator it = A.value.begin();

    for (auto& val: ret_vec) {

        val=*it - B.get();
        ++it;
    }

    return {ret_vec};      
}

Const operator*(const Vector& A, const Vector& B) {

    if(A.value.size()!=B.value.size()) throw VecrtorSize();
    double summ=0.0;
    std::vector<double>::const_iterator it1 = A.value.begin();
    std::vector<double>::const_iterator it2 = B.value.begin();

    while (it1!=A.value.end()) {

        summ += *it1 * *it2;
        ++it1; ++it2;
    }
 
    return {summ};
}

Vector operator*(Const A, const Vector& B) {

    std::vector<double> ret_vec;
    ret_vec.resize(B.value.size());
    std::vector<double>::const_iterator it = B.value.begin();

    for (auto& val: ret_vec) {

        val=A.get() * *it;
        ++it;
    }

    return {ret_vec}; 
}

Vector operator/(Const A, const Vector& B) {

    std::vector<double> ret_vec;
    ret_vec.resize(B.value.size());
    std::vector<double>::const_iterator it = B.value.begin();

    for (auto& val: ret_vec) {

        val=A.get() / *it;
        ++it;
    }

    return {ret_vec}; 
}

Vector operator/(const Vector& A, Const B) {

    std::vector<double> ret_vec;
    ret_vec.resize(A.value.size());
    std::vector<double>::const_iterator it = A.value.begin();

    for (auto& val: ret_vec) {

        val=*it / B.get();
        ++it;
    }

    return {ret_vec}; 
}