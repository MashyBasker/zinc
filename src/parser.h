#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "lexer.h"

/* Return type for each node of the AST */
struct ASTNode {
    std::string type;
    std::unordered_map<std::string, std::vector<ASTNode>> children;
    std::string value;
};

/* Struct storing parser information */
struct Parser {
    const std::vector<Token> *tokens = nullptr;
    size_t currIndex = 0;

    // initialize the tokens
    void setTokens(const std::vector<Token> &tokenStream) {
        tokens = &tokenStream;
        currIndex = 0;

    }
};

ASTNode parseProgram(Parser &parser);
ASTNode parseStatementList(Parser &parser);
ASTNode parseStatement(Parser &parser);
ASTNode parseDeclaration(Parser &parser);
ASTNode parseAssignment(Parser &parser);
ASTNode parseExpression(Parser &parser);
ASTNode parseExpressionTail(Parser &parser, ASTNode left);
ASTNode parseConditional(Parser &parser);
ASTNode parseCondition(Parser &parser);
ASTNode parseIdentifier(Parser &parser);
ASTNode parseNumber(Parser &parser);
ASTNode parseTerm(Parser &parser);
bool isIdentifier(const std::string& token);
bool isNumber(const std::string& token);
void displayAST(const ASTNode &node, int indentLevel);

#endif // PARSER_H