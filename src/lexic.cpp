#include <set>
#include "lexic.h"
#include "log.h"

enum State {S_INPUT, S_CONST, S_VEC_INPUT, S_SYMB, S_VAR};       // Posible states of finite state machine

static std::string buffer;
static State flag = S_INPUT;
static const std::set<char> operators {'+', '-', '*', '/', '='};
static const std::set<char> service {'(', ')', ',', '{', '}', ';', '~'};
static std::vector<double> vec;

void TokenStream::read() {

    LOGI("\nLexical analisys:\tSize %zu; Cursor %zu", size(), it)
    for (char ch; input>>ch;) {

        if ((flag == S_INPUT || flag == S_CONST) && (ch >= '0' && ch <= '9') || (ch == '.')) {  

            flag = S_CONST;
            buffer += ch;
        }
        else if ((flag==S_INPUT || flag==S_VAR) && (ch>='a' && ch<='z')) {

            flag = S_VAR;
            buffer += ch;

        }
        else if (flag == S_CONST) {    

            tokens.push_back(new Const(std::stod(buffer)));

            flag = S_INPUT;
            input.putback(ch);     
            buffer.clear();
        }
        else if (flag == S_VAR) {

            tokens.push_back(new Variable(buffer.c_str()));

            flag = S_INPUT;
            input.putback(ch);
            buffer.clear();
        }
        else if (operators.find(ch) != operators.end()) {   

            tokens.push_back((Token*)&operators.at(ch));
        }        
        else if (service.find(ch) != service.end()) {
            
            switch (ch) {
                case '{': flag=S_VEC_INPUT; vec.clear(); break;
                case ',': vec.push_back(std::stod(buffer)); buffer.clear(); break;
                //case '}': vec.push_back(std::stod(buffer)); tokens.push_back(new Vector(vec)); flag=S_INPUT; buffer.clear(); break; 
                case '(': tokens.push_back(&op); break;
                case ')': tokens.push_back(&cp); break;
                case ';': tokens.push_back(&ee); return;
                case '~': throw Quit();
            }
        }
        else {
            LOGE("Invalid token: %c\n", ch);
            throw InvalidToken();    
        }                
    }
}