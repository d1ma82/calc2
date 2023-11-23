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

    if(rule.size()==0) return "{}";

    std::stringstream ret;
    ret << "{";
    auto i=rule.size();
    for (auto elem = rule.crbegin(); elem != rule.crend(); ++elem)  ret << word_to_str(*elem) << (--i==0? '}': ',');
    return ret.str();
}

static Word token_to_word(Token* token) {

    switch (token->type) {
        case T_CONST:  return W_CONST;
        case T_VECTOR: return W_VECTOR;
        case T_VAR:    return W_VAR; 
        case T_OPER: {
            switch (dynamic_cast<OperatorToken*>(token)->get()) {
                case '+': return W_PLUS;
                case '-': return W_MINUS;   
                case '*': return W_PRODUCT;   
                case '/': return W_DIV;
                case '=': return W_ASSIGN;                
                default: return W_UDEFINED;
            }
        }
        case T_OP:  return W_OPEN_PARENTH;
        case T_CP:  return W_CLOSE_PARENTH;
        case T_EOEQ: return W_END_EQUATION;
        default:    return W_UDEFINED;
    }
}

void Synt::read() {

    LOGI("\nSyntax analisys:\n")
    rules.push({W_E});
    Token* token;
    *input>>token;

    while (!input->eof()) {

      bool result = check(token);
      prev=token;
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
        Word word = rule.empty() ? W_UDEFINED : *rule.rbegin();
        rule.pop_back();

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
                    case T_OPER: {

                        OperatorToken* op = dynamic_cast<OperatorToken*>(token);
                        if (op->get()=='-' && (prev==nullptr || prev->type==T_OP || prev->type==T_OPER)) {
                            op->set_unar();
                        }
                    }
                    default: break;
                }
                return word==token_to_word(token);
        }
    }
    return false;
}

// Make table values vice versa for easy earasing elements
void Synt::init_table() {
    // initialize rule set following table
//               c  vecP    var   +   -   *   /   ;  (    )    =    
//            E cP  vecP   varK      -E           ;  (E                                
//            P                   +E -E   *E  /E  ;       )P      
//            K                   +E -E   *E  /E  ;  (E   )P   =E
    table[E][W_CONST]           = {W_P, W_CONST};
    table[E][W_VECTOR]          = {W_P, W_VECTOR};
    table[E][W_VAR]             = {W_K, W_VAR};
    table[E][W_END_EQUATION]    = {W_END_EQUATION};
    table[P][W_END_EQUATION]    = {W_END_EQUATION};
    table[E][W_MINUS]           = {W_E, W_MINUS};  
    table[P][W_PLUS]            = {W_E, W_PLUS};
    table[P][W_MINUS]           = {W_E, W_MINUS};
    table[P][W_PRODUCT]         = {W_E, W_PRODUCT};
    table[P][W_DIV]             = {W_E, W_DIV};
    table[E][W_OPEN_PARENTH]    = {W_E, W_OPEN_PARENTH};
    table[P][W_CLOSE_PARENTH]   = {W_P, W_CLOSE_PARENTH};
    table[K][W_PLUS]            = {W_E, W_PLUS};
    table[K][W_MINUS]           = {W_E, W_MINUS};
    table[K][W_DIV]             = {W_E, W_DIV};
    table[K][W_PRODUCT]         = {W_E, W_PRODUCT};
    table[K][W_OPEN_PARENTH]    = {W_E, W_OPEN_PARENTH};
    table[K][W_CLOSE_PARENTH]   = {W_P, W_CLOSE_PARENTH};
    table[K][W_ASSIGN]          = {W_E, W_ASSIGN};
    table[K][W_END_EQUATION]    = {W_END_EQUATION};
}