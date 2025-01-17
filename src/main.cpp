#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "lexer.h"
#include "parser.h"
#include "codegen.h"

std::string getOutputFileName(const std::string& inputFile) {
    size_t lastDot = inputFile.find_last_of('.');
    if (lastDot == std::string::npos) {
        return inputFile + ".asm"; // No extension, just add .asm
    }
    return inputFile.substr(0, lastDot) + ".asm";
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    }

    std::vector<Token> tokens;
    const std::string file = argv[1];
    const std::string outfile = getOutputFileName(file);
    tokens = tokenizeFile(file);

    Parser parser;
    parser.setTokens(tokens);

    // for(auto t: tokens) {
    //     std::cout << t.text << " <--> " << t.type << std::endl;
    // }

    try {

        ASTNode ast = parseProgram(parser);
        // displayAST(ast, 0);
        auto assembly = generateProgram(ast);
        std::ofstream outFile(outfile);
        if (!outFile) {
            throw std::runtime_error("Could not open output file: " + outfile);
        }

        for (const auto& line : assembly) {
            outFile << line << std::endl;
        }
    } catch(const std::runtime_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
    }

    return 0;
}