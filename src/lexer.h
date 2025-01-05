#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

/* Define the types of the tokens */
typedef enum {
    TOKEN_INT,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_ASSIGN,
    TOKEN_ADD,
    TOKEN_SUBTRACT,
    TOKEN_IF,
    TOKEN_EQUAL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokType;

/* Define a struct type for the tokens*/
typedef struct {
    std::string text;
    TokType type;
} Token;

std::vector<Token> tokenizeFile(const std::string filePath);

#endif