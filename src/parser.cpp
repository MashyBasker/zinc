#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cctype>
#include <algorithm>

#include "parser.h"


/*
Implementing a recursive descent parser for Simple Lang.
*/

ASTNode parseProgram(Parser &parser) {
    // <program> ::= <statement_list>
    return parseStatementList(parser);
}

ASTNode parseStatementList(Parser &parser) {
    std::vector<ASTNode> statements;

    // Continue parsing until a '}' or EOF is encountered
    while (parser.currIndex < (*parser.tokens).size() &&
           (*parser.tokens)[parser.currIndex].text != "}") {
        // Parse the current statement
        statements.push_back(parseStatement(parser));
    }

    // // After parsing, check if we've encountered a '}' and move past it
    // if (parser.currIndex < (*parser.tokens).size() && (*parser.tokens)[parser.currIndex].text == "}") {
    //     parser.currIndex++;  // Move past '}'
    // } else if (parser.currIndex >= (*parser.tokens).size()) {
    //     // If we reached the end of the token list without encountering '}', throw an error
    //     throw std::runtime_error("Unexpected end of input: expected '}'");
    // }

    // Return the list of statements as part of the statement_list node
    return ASTNode{"statement_list", {{"statements", statements}}, ""};
}

void displayAST(const ASTNode &node, int indentLevel = 0) {
    std::string indent(indentLevel, ' ');
    std::cout << indent << "Node Type: " << node.type << std::endl;
    if(!node.value.empty()) {
        std::cout << indent << "Value: " << node.value << std::endl;
    }

    for(const auto &child: node.children) {
        std::cout << indent << "Children: " << child.first << std::endl;
        for(const auto &childNode: child.second) {
            displayAST(childNode, indentLevel+2);
        }
    }
}

ASTNode parseStatement(Parser &parser){
    // <statement> ::= <declaration> | <assignment> | <conditional>
    if((*parser.tokens)[parser.currIndex].text == "int") {
        // for: int a;
        return parseDeclaration(parser);
    } else if((*parser.tokens)[parser.currIndex].text == "if") {
        // for: if(a == 1) { b = b - 1; }
        return parseConditional(parser);
    } else if(isIdentifier((*parser.tokens)[parser.currIndex].text)) {
        // for: a = 6;
        // distinguish between assignment and other constructs
        size_t nextIndex = parser.currIndex + 1;
        if(nextIndex < (*parser.tokens).size() && (*parser.tokens)[nextIndex].text == "=") {
            return parseAssignment(parser);
        } else {
            throw std::runtime_error("Semantic error: Unexpected token after identifier: '" + (*parser.tokens)[nextIndex].text + "'");
        }
    } else if((*parser.tokens)[parser.currIndex].type == 14) {
        // reached EOF
        parser.currIndex++;
        return ASTNode{"eof", {}, ""};
    } else {
        throw std::runtime_error("Syntax error: Unexpected token '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
}

ASTNode parseDeclaration(Parser &parser) {
    // <declaration> ::= "int" <identifier> ";"
    if((*parser.tokens)[parser.currIndex].text != "int") {
        throw std::runtime_error("Syntax error: Expected 'int' got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    parser.currIndex++; // move to next token if `int` is matched
    ASTNode id = parseIdentifier(parser);
    if((*parser.tokens)[parser.currIndex].text != ";") {
        throw std::runtime_error("Syntax error: Missing ';'");
    }
    parser.currIndex++;
    return {"declaration", {}, id.value};
}

ASTNode parseAssignment(Parser& parser) {
    // <assignment> ::= <identifier> "=" <expression> ";"
    ASTNode id = parseIdentifier(parser);
    if((*parser.tokens)[parser.currIndex].text != "=") {
        throw std::runtime_error("Syntax error: Expected '=' found '" + (*parser.tokens)[parser.currIndex].text);
    }
    parser.currIndex++;
    ASTNode expr = parseExpression(parser);
    if((*parser.tokens)[parser.currIndex].text != ";") {
        throw std::runtime_error("Syntax error: Missing `;`");
    }
    parser.currIndex++;
    return {"assignment", {{"expression", {expr}}}, id.value};
}

ASTNode parseExpression(Parser &parser) {
    // <expression> ::= <term> <expression_tail>
    ASTNode term = parseTerm(parser);
    return parseExpressionTail(parser, term);
}

ASTNode parseExpressionTail(Parser &parser, ASTNode left) {
    // <expression_tail> ::= "+" <term> <expression_tail> | "-" <term> <expression>
    if(parser.currIndex < (*parser.tokens).size()) {
        if((*parser.tokens)[parser.currIndex].text == "+"
           || (*parser.tokens)[parser.currIndex].text == "-") {
            std::string op = (*parser.tokens)[parser.currIndex++].text; // the operator
            ASTNode right = parseTerm(parser);
            return parseExpressionTail(parser, {
                "binary_op",
                {
                    {"right", {right}},
                    {"left",  {left}}
                },
                op
            });
        }
    }
    return left;
}

ASTNode parseTerm(Parser &parser) {
    // <term> ::= <identifier> | <number>
    if(isIdentifier((*parser.tokens)[parser.currIndex].text)) {
        return parseIdentifier(parser);
    } else if(isNumber((*parser.tokens)[parser.currIndex].text)) {
        return parseNumber(parser);
    } else {
        throw std::runtime_error("Syntax error: Expected a number or identifier here, got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
}

ASTNode parseIdentifier(Parser &parser) {
    // <identifier> ::= <letter> <identifier_tail>
    if(!std::isalpha((*parser.tokens)[parser.currIndex].text[0])) {
        throw std::runtime_error("Invalid identifier: Starts with '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    ASTNode node = {
        "identifier",
        {},
        (*parser.tokens)[parser.currIndex++].text
    };
    return node;
}

ASTNode parseNumber(Parser &parser) {
    // <number> ::= <digit> <number_tail>
    if(!std::isdigit((*parser.tokens)[parser.currIndex].text[0])) {
        throw std::runtime_error("Invalid number: Starts with '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    ASTNode node = {
        "number",
        {},
        (*parser.tokens)[parser.currIndex++].text
    };
    return node;
}

ASTNode parseConditional(Parser &parser) {
    // <conditional> ::= "if" "(" <condition> ")" "{" <statement_list> "}"
    if((*parser.tokens)[parser.currIndex].text != "if") {
        throw std::runtime_error("Expected 'if' got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    parser.currIndex++;

    if((*parser.tokens)[parser.currIndex].text != "(") {
        throw std::runtime_error("Expected '(' got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    parser.currIndex++;

    ASTNode condition = parseCondition(parser);

    if((*parser.tokens)[parser.currIndex].text != ")") {
        throw std::runtime_error("Expected ')' got '" + (*parser.tokens)
        [parser.currIndex].text + "'");
    }
    parser.currIndex++;
    if((*parser.tokens)[parser.currIndex].text != "{") {
        throw std::runtime_error("Expected '{' got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    parser.currIndex++;

    ASTNode body = parseStatementList(parser);

    if((*parser.tokens)[parser.currIndex].text != "}") {
        throw std::runtime_error("Expected '}' got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    parser.currIndex++;
    return {
        "conditional",
        {{"condition", {condition}},
        {"body", {body}}},
        ""
    };
}

ASTNode parseCondition(Parser &parser) {
    // <condition> ::= <expression> "==" <expression>
    ASTNode left = parseExpression(parser);
    if((*parser.tokens)[parser.currIndex].text != "==") {
        throw std::runtime_error("Expected '==' got '" + (*parser.tokens)[parser.currIndex].text + "'");
    }
    parser.currIndex++;
    ASTNode right = parseExpression(parser);
    return {
        "condition",
        {{"left", {left}},
        {"right", {right}}},
        "=="
    };
}

bool isIdentifier(const std::string &token) {
    return !token.empty() && std::isalpha(token[0]);
}

bool isNumber(const std::string &token) {
    return !token.empty() && std::all_of(token.begin(), token.end(), isdigit);
}

