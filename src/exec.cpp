#include "exec.h"


void Executor::exec_internal(Token* A, Token* B, char operation) {

    if (!A) LOGI("Expression:\t%c%s", operation, B->str())
    else LOGI("Expression:\t%s%c%s", A->str(), operation, B->str())

    switch (B->type) {
        case T_CONST:   
            if (!A) exec_unar<Const>(*(dynamic_cast<Const*>(B)), operation);
            else if (A->type==T_CONST) exec_dbl<Const, Const, Const>(A, B, operation);
            else if (A->type==T_VECTOR) exec_dbl<Vector, Const, Vector>(A, B, operation);
            else if (A->type==T_VAR) {

                    if (operation=='=') assign(A, B); 
                    else exec_internal(var_map[dynamic_cast<Variable*>(A)->str()], B, operation);
            }
            break;
        case T_VECTOR:
            if (!A) exec_unar<Vector>(*(dynamic_cast<Vector*>(B)), operation);
            else if (A->type==T_CONST) exec_dbl<Const, Vector, Vector>(A, B, operation);
            else if (A->type==T_VAR) {

                    if (operation=='=') assign(A, B);
                    else exec_internal(var_map[dynamic_cast<Variable*>(A)->str()], B, operation); 
            }
            else if (A->type==T_VECTOR) {
                switch (operation) {
                    case '+': case '-': exec_vec_spec1<Vector, Vector, Vector>(A, B, operation); break;
                    case '*': case '/': exec_vec_spec2<Vector, Vector, Const>(A, B, operation); break;
                }                
            }
            break;
        case T_VAR:
            if (!A) exec_internal(nullptr, var_map[dynamic_cast<Variable*>(B)->str()], operation);
            else exec_internal(A, var_map[dynamic_cast<Variable*>(B)->str()], operation);
            break;
        default: LOGD("Unknown Token type") break;
    }
}

void Executor::do_exec(char operation, bool unar) {

    Token* B{nullptr};
    Token* A{nullptr};

    B = stack.top(); stack.pop();

    if (!unar) { A = stack.top(); stack.pop(); }

    exec_internal(A, B, operation);
}

void Executor::read() {

    LOGI("\nExecutor:\tSize %zu; Cursor %zu", size(), it)

    Token* token;
    *input>>token;

    while (!input->eof()) {
        
        switch (token->type) {
            case T_CONST: case T_VECTOR: case T_VAR:  stack.push(token); break;
            case T_OPER: do_exec(dynamic_cast<OperatorToken*>(token)->get(), 
                                 dynamic_cast<OperatorToken*>(token)->is_unar()); break;
            default: break;
        }
        *input>>token;
    }
}