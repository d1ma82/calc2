#include "syntax.h"
#include "token.h"
#include "log.h"


static const int E=0, P=1, K=2;

static const char* word_to_str(Word word) {
    switch (word) {

            case  W_CONST:          return "CONST";
            case  W_VECTOR:         return "VECTOR";
            case  W_VAR:            return "VARIABLE";
            case  W_PLUS:           return "PLUS";
            case  W_MINUS:          return "MINUS";
            case  W_PRODUCT:        return "PRODUCT";
            case  W_DIV:            return "DIV";
            case  W_END_EQUATION:   return "END_EQUATION";
            case  W_UDEFINED:       return "UDEFINED";
            case  W_OPEN_PARENTH:   return "OPEN_PARENTH";
            case  W_CLOSE_PARENTH:  return "CLOSE_PARENTH";
            case  W_ASSIGN:         return "ASSIGN";
            case  W_E:              return "E";
            case  W_P:              return "P";
            case  W_K:              return "K";
            default :               return "UNKNOWN";
    }
}

static std::string rule_to_string(Rule rule) {

    if (rule.size()==0) return "{}";

    std::stringstream ret; 
    ret << "{";
    ret << word_to_str(rule.front());
    if (rule.size()>1) ret << ',' << word_to_str(rule.back());
    ret << '}';
    return ret.str();
}

static Word token_to_word(Token* token) {

    switch (token->type) {
        case T_CONST:   return W_CONST;
        case T_VECTOR:  return W_VECTOR;
        case T_VAR:     return W_VAR; 
        case T_PLUS:    return W_PLUS;
        case T_MINUS:   return W_MINUS;
        case T_PRODUCT: return W_PRODUCT;
        case T_DIV:     return W_DIV;
        case T_ASSIGN:  return W_ASSIGN;
        case T_OP:      return W_OPEN_PARENTH;
        case T_CP:      return W_CLOSE_PARENTH;
        case T_EOEQ:    return W_END_EQUATION;
        default:        return W_UDEFINED;
    }
}

void Synt::read() {

    LOGI("\nSyntax analisys:\n")
    rules.push(begin);
    Token* token;
    *input>>token;

    while (!input->eof()) {

      bool result = check(token);
      if (!result) {LOGE("Syntax error\n") throw SyntaxException();}
      *input>>token;
    }
    if (cnt_op != cnt_cp) { LOGE("Check parenthesis") throw ParenthesisException(); }
}

// check syntax by syntax table
bool Synt::check(Token* token) {
    
    Rule rule = rules.top();

    while (!rules.empty()) {    

        rules.pop();
        LOGI("Rule: %s\tType: %s", rule_to_string(rule).c_str(), type_to_str(token->type))
        Word word = rule.empty() ? W_UDEFINED : rule.front();
        rule.pop();

        switch (word) {
            case W_E:
                rules.push(table[E][token_to_word(token)]);
                rule = rules.top();
                if (rule.empty()) return false;
                break;

            case W_P:
                rules.push(table[P][token_to_word(token)]);
                rule = rules.top();
                if (rule.empty()) return false;
                break;
            
            case W_K:
                rules.push(table[K][token_to_word(token)]);
                rule = rules.top();
                if (rule.empty()) return false;
                break;

            default:
                   
                rules.push(rule);
                switch (token->type) {
                    case T_OP: cnt_op++; break;
                    case T_CP: cnt_cp++; break;
                    default: break;
                }
                return word==token_to_word(token);
        }
    }
    return false;
}

void Synt::init_table() {
    // initialize rule set following table
//               c  vecP    var   +   -   *   /   ;  (    )    =    
//            E cP  vecP   varK      -E           ;  (E                                
//            P                   +E -E   *E  /E  ;       )P      
//            K                   +E -E   *E  /E  ;       )P   =E
    table[E][W_CONST]        .push(W_CONST);
    table[E][W_CONST]        .push(W_P);
    table[E][W_VECTOR]       .push(W_VECTOR);
    table[E][W_VECTOR]       .push(W_P);
    table[E][W_VAR]          .push(W_VAR);
    table[E][W_VAR]          .push(W_K);   
    table[E][W_END_EQUATION] .push(W_END_EQUATION);
    table[P][W_END_EQUATION] .push(W_END_EQUATION);
    table[E][W_MINUS]        .push(W_MINUS);
    table[E][W_MINUS]        .push(W_E);  
    table[P][W_PLUS]         .push(W_PLUS);
    table[P][W_PLUS]         .push(W_E);
    table[P][W_MINUS]        .push(W_MINUS);
    table[P][W_MINUS]        .push(W_E);
    table[P][W_PRODUCT]      .push(W_PRODUCT);
    table[P][W_PRODUCT]      .push(W_E);
    table[P][W_DIV]          .push(W_DIV);
    table[P][W_DIV]          .push(W_E);
    table[E][W_OPEN_PARENTH] .push(W_OPEN_PARENTH);
    table[E][W_OPEN_PARENTH] .push(W_E);
    table[P][W_CLOSE_PARENTH].push(W_CLOSE_PARENTH);
    table[P][W_CLOSE_PARENTH].push(W_P);
    table[K][W_PLUS]         .push(W_PLUS);
    table[K][W_PLUS]         .push(W_E);   
    table[K][W_MINUS]        .push(W_MINUS);
    table[K][W_MINUS]        .push(W_E);
    table[K][W_DIV]          .push(W_DIV);
    table[K][W_DIV]          .push(W_E);
    table[K][W_PRODUCT]      .push(W_PRODUCT);
    table[K][W_PRODUCT]      .push(W_E);
    table[K][W_CLOSE_PARENTH].push(W_CLOSE_PARENTH);
    table[K][W_CLOSE_PARENTH].push(W_P);
    table[K][W_ASSIGN]       .push(W_ASSIGN);
    table[K][W_ASSIGN]       .push(W_E);
    table[K][W_END_EQUATION] .push(W_END_EQUATION);
}