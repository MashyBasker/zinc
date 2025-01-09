#include <iostream>
#include <vector>
#include <string>

#include "lexer.h"
#include "parser.h"


int main() {

    std::vector<Token> tokens;
    const std::string file = "./data/arithmetic.sl";
    tokens = tokenizeFile(file);

    // for(auto t: tokens) {
        // std::cout << t.text << "  <<>>  " << t.type << std::endl;
    // }
    Parser parser;
    parser.setTokens(tokens);

    try {

        ASTNode ast = parseProgram(parser);
        std::cout << "Parsed successfully!" << std::endl;
        displayAST(ast, 0);
    } catch(const std::runtime_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }

    return 0;
}