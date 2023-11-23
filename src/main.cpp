#include <iostream>
#include "lexic.h"
#include "syntax.h"
#include "postfix.h"
#include "exec.h"
#include "token.h"
#include "log.h"

static TokenStream lexic(std::cin);
static Synt syntax(&lexic);
static Postfix postfix(&lexic);
static Executor exe(&postfix);
static int ret=0;

static bool yes() {

    char ch;
    std::cin.clear();
    std::cin.sync();
    std::cin >> ch;

    return ch == 'y';
}

static void print(Stream* stream) {
    
    Token* next;
    *stream>>next;

    while (!stream->eof()) {

        std::cout << type_to_str(next->type) <<"\t"<< next->str() << std::endl;
        *stream>>next;
    }     
}

static void init() {

    LOGI("Enter expression(s) like '1+2;' press ~ to quit")
    LOGI("You can use vectors in expressions like {1,2,3}+5")
    LOGI("For variables use lower case characters like : width=500")
}

static void loop() {

    do {
        int l_pos = 0, p_pos = 0;
        try {
            while (true) {

                lexic.read();
                print(&lexic);
                lexic.seekp(l_pos);
                syntax.read();
                lexic.seekp(l_pos);
                postfix.read();
                print(&postfix);
                postfix.seekp(p_pos);
                exe.read();
                print(&exe);

                l_pos = lexic.pos();
                p_pos = postfix.pos();
            }
        }
        catch (Quit) { ret=0; return; }
        catch (ParenthesisException) { ret= -1; } 
        catch (SyntaxException) { ret = -2; }
        catch (InvalidToken) { ret = -3; }
        catch (VecrtorSize) { ret = -4; }
        catch (std::exception const &e) {
            LOGE("%s", e.what())
            ret = -10;
        }

        lexic.clear();
        postfix.clear();
        exe.clear();
        LOGI("Continue? y/n")
    } while (yes());
}

static void clear() {}

int main() {

    init();
    loop();
    clear();
    LOGI("Quit")
    return ret;
}