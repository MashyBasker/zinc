#include <iostream>
#include <vector>
#include <string>

#include "lexer.h"


int main() {

    std::vector<Token> tokens;
    const std::string file = "./data/prog.sl";
    tokens = tokenizeFile(file);

    for(auto t: tokens) {
        std::cout << t.text << t.type << std::endl;
    }
    return 0;
    
}