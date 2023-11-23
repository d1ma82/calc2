#include "postfix.h"

static int priority(Token* token) {

    if (token->type!=T_OPER) return 0;
    
    switch (dynamic_cast<OperatorToken*>(token)->get()) {
        case '+': return 3;
        case '-': return dynamic_cast<OperatorToken*>(token)->is_unar()? 1: 3;   
        case '*': return 2;   
        case '/': return 2;
        case '=': return 4;
        default: return 0;
    }
}

void Postfix::close_parenthesis() {

    if (stack.empty()) return;

    while (stack.top()->type!=T_OP) {
        
        vec.push_back(stack.top());
        stack.pop();
        if (stack.empty()) return;
    }
    stack.pop();    // pop '('
}

void Postfix::pop_if_lower(int curr_prioprity) {

    if (stack.empty() || curr_prioprity==0) return;

    while (priority(stack.top())>0 && priority(stack.top())<=curr_prioprity) {    

        vec.push_back(stack.top());
        stack.pop();
        if (stack.empty()) return;
    }
}

void Postfix::pop_until_empty() {

    while (!stack.empty()) {

        vec.push_back(stack.top());
        stack.pop();
    }
}

void Postfix::read() {

    Token* token;
    *input>>token;
    int old_priority {0}, curr_priority {0};
    LOGI("\nPostfix:\tSize %zu; Cursor %zu", size(), it)

    while (!input->eof()) {

        curr_priority = priority(token);
        old_priority  = stack.empty()? 0: priority(stack.top());

        switch (token->type) {
            
            case T_CONST: case T_VECTOR: case T_VAR: vec.push_back(token); break;
            case T_OP:  stack.push(token); break;
            case T_CP:  close_parenthesis(); break;
            case T_EOEQ: pop_until_empty(); break;
            case T_OPER: 

                if (stack.empty()) stack.push(token);  
                else if (old_priority>0 && old_priority<=curr_priority) { pop_if_lower(curr_priority); stack.push(token); } 
                else stack.push(token); 
        }
        *input>>token;
    }
}