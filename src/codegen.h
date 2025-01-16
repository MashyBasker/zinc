#ifndef CODEGEN_H
#define CODEGEN_H

#include <vector>
#include <unordered_map>
#include <string>
#include <string>
#include <stdexcept>

#include "parser.h"

std::string generateUniqueLabel(const std::string &base);

std::vector<std::string> generateDeclaration(const ASTNode &node, std::unordered_map<std::string, int> &symbolTable, int &nextVarAddress);

std::vector<std::string> generateAssignment(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable);

std::vector<std::string> generateExpression(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable, const std::string &reg="A");

std::vector<std::string> generateCondition(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable);

std::vector<std::string> generateIf(const ASTNode &node, const std::unordered_map<std::string, int> &symbolTable);

std::vector<std::string> generateProgram(const ASTNode &ast);

#endif