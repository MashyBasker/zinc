#include <iostream>
#include <vector>
#include <string>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"


int main() {

    std::vector<Token> tokens;
    const std::string file = "./data/assign.sl";
    tokens = tokenizeFile(file);

    for(auto t: tokens) {
        std::cout << t.text << "  <<>>  " << t.type << std::endl;
    }
    Parser parser;
    parser.setTokens(tokens);

    try {

        ASTNode ast = parseProgram(parser);
        std::cout << "Parsed successfully!" << std::endl;
        displayAST(ast, 0);
        std::vector<std::string> assembly = generate_program(ast);
        // for(auto a: assembly) {
        //     std::cout << a << std::endl;
        // }
    } catch(const std::runtime_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }

    return 0;
}