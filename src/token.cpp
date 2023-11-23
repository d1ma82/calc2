#include "token.h"

const char* type_to_str(Type type) {

    switch (type) {
        
        case T_CONST:   return "const";
        case T_VECTOR:  return "vector";
        case T_VAR:     return "variable";
        case T_OPER:    return "operator";
        case T_OP:      return "open par";
        case T_CP:      return "close par";
        case T_EOEQ:    return "end of equation";    
    }
}