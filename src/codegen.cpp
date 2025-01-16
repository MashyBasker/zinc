#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include "parser.h"


std::vector<std::string> generateDeclaration(const ASTNode &node, std::unordered_map<std::string, int> &symbolTable, int &nextVarAddress) {
    std::vector<std::string> code;
    if(node.value.empty()) {
        throw std::runtime_error("Invalid declaration: No variable name");
    }

    // adding variable to the symbol table
    symbolTable[node.value] = nextVarAddress;
    code.push_back(node.value + " = " + std::to_string(nextVarAddress));
    nextVarAddress--;
    return code;
}

std::vector<std::string> generateExpression(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable, const std::string &reg="A") {
    std::vector<std::string> code;

    if(node.type == "number") {
        code.push_back("ldi " + reg + " " + node.value);
    }
    else if(node.type == "identifier") {
        if(symbolTable.find(node.value) == symbolTable.end()) {
            throw std::runtime_error("Undefined variable: " + node.value);
        }
        code.push_back("mov " + reg + " M %" + node.value);
    }
    else if(node.type == "binary_op") {
        if(node.children.find("left") == node.children.end() || node.children.find("right") == node.children.end()) {
            throw std::runtime_error("Binary operation must have 'left' and 'right' operand");
        }
        std::vector<std::string> left = generateExpression(node.children.at("left")[0], symbolTable);
        std::vector<std::string> right = generateExpression(node.children.at("right")[0], symbolTable, "B");

        code.insert(code.end(), left.begin(), left.end());
        code.insert(code.end(), right.begin(), right.end());

        if(node.value == "+") {
            code.push_back("add");
        }
        else if(node.value == "-") {
            code.push_back("sub");
        }
        else {
            throw std::runtime_error("Unsupported binary operation: " + node.value);
        }
    } else {
        throw std::runtime_error("Unsupported node type: " + node.type);
    }
    return code;
}

std::string generateUniqueLabel(const std::string &base) {
    static int counter = 0; // this is very bad, but it will do for now
    std::ostringstream label;
    label << base << "_" << counter++;
    return label.str();
}

std::vector<std::string> generateCondition(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable) {
    std::vector<std::string> code;
    if(node.value == "==") {
        std::vector<std::string> leftCode = generateExpression(node.children.at("left")[0], symbolTable, "A");
        std::vector<std::string> rightCode = generateExpression(node.children.at("right")[0], symbolTable, "B");

        code.insert(code.end(), leftCode.begin(), leftCode.end());
        code.insert(code.end(), rightCode.begin(), rightCode.end());
    } else {
        throw std::runtime_error("Unsupported condition operation: " + node.value);
    }
    return code;
}


std::vector<std::string> generateAssignment(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable) {
    std::vector<std::string> code;
    std::string varName = node.value; // variable for assignment

    // check if it exists
    if(symbolTable.find(varName) == symbolTable.end()) {
        throw std::runtime_error("Undefined variable: " + varName);
    }

    if(node.children.find("expression") == node.children.end()) {
        std::runtime_error("Assignment node missing 'expression' child");
    }

    const ASTNode &expression = node.children.at("expression")[0];
    auto expressionCode = generateExpression(expression, symbolTable, "A");
    code.insert(code.end(), expressionCode.begin(), expressionCode.end());

    code.push_back("mov M A %" + varName); // store the variable data in memory
    return code;
}

std::vector<std::string> generateIf(const ASTNode &node, std::unordered_map<std::string, int> &symbolTable) {
    std::vector<std::string> code;
    std::string iftrueLabel = generateUniqueLabel("if_true");
    std::string ifendLabel = generateUniqueLabel("if_end");

    const ASTNode &conditionNode = node.children.at("condition")[0];
    std::vector<std::string> conditionCode = generateCondition(conditionNode, symbolTable);

    code.insert(code.end(), conditionCode.begin(), conditionCode.end());

    code.push_back("je %" + iftrueLabel); // jump here if A == B
    code.push_back("jmp %" + ifendLabel); // jump here if A != B

    // generate code for if-block
    code.push_back(iftrueLabel + ":");
    const auto &blockStatements = node.children.at("body")[0];
    if(blockStatements.type == "statement_list") {
        const auto &bodyStatements = blockStatements.children.at("statements");
        for(const auto &stmt : bodyStatements) {
            if(stmt.type == "assignment") {
                auto stmtCode = generateAssignment(stmt, symbolTable);
                code.insert(code.end(), stmtCode.begin(), stmtCode.end());
            }
            else if(stmt.type == "declaration") {
                continue;
                // does not handle declarations as of now
            }
            else {
                throw std::runtime_error("Unsupported statement type in body: " + stmt.type);
            }
        }
    } else {
        throw std::runtime_error("Body is not a statement_list");
    }
    code.push_back(ifendLabel + ":");
    return code;
}


std::vector<std::string> generateProgram(const ASTNode &ast) {
    // maintain a symbol table to track all declared variables
    std::unordered_map<std::string, int> symbolTable;

    // data addresses start from 255 and decrease. memory => 255 bytes
    int nextVarAddress = 255;

    // store all the code in vec of strings
    std::vector<std::string> code;

    code.push_back(".data\n");

    auto stmtList = ast.children.find("statements");
    if(stmtList == ast.children.end()) {
        throw std::runtime_error("Error: 'statements' not found in AST children\n");
    }

    for(const auto &stmt : stmtList->second) {
        if(stmt.type == "declaration") {
            auto declCode = generateDeclaration(stmt, symbolTable, nextVarAddress);
            code.insert(code.end(), declCode.begin(), declCode.end());
            nextVarAddress--;
        }
    }

    code.push_back("\n.text\n");

    for(const auto &stmt : stmtList->second) {
        if(stmt.type == "assignment") {
            auto assignCode = generateAssignment(stmt, symbolTable);
            code.insert(code.end(), assignCode.begin(), assignCode.end());
        }
        else if(stmt.type == "conditional") {
            auto ifCode = generateIf(stmt, symbolTable);
            code.insert(code.end(), ifCode.begin(), ifCode.end());
        }
    }

    code.push_back("\nhlt");
    return code;

}