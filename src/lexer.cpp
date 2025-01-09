#include <cctype>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include "lexer.h"

/* Function takes the input file stream and returns
the next token. Each of these tokens will be added to
vector for tokens. */
Token getNextToken(std::ifstream &fileStream) {
    char ch;
    Token token;
    token.text.clear();
    while(fileStream.get(ch)) {
        if(std::isspace(ch)) {
            // ignore empty spaces
            continue;
        }
        
        // now for alphanumeric chars
        if(std::isalpha(ch)) {
            token.text = ch;
            while(fileStream.get(ch) && (std::isalnum(ch))) {
                token.text += ch;
            }
            fileStream.unget();

            if(token.text == "if") {
                token.type = TOKEN_IF;
            } else if(token.text == "int") {
                token.type = TOKEN_INT;
            } else {
                token.type = TOKEN_IDENTIFIER;
            }

            return token;
        }

        // checking for numbers
        if(std::isdigit(ch)) {
            token.text = ch;
            while(fileStream.get(ch)) {
                if(std::isdigit(ch)) {
                    token.text += ch;
                } else {
                    fileStream.unget();
                    break;
                }
            }
            token.type = TOKEN_NUMBER;
            return token;
        }
        
        // checking for single character tokens
        switch(ch) {
            case '=':
                token.text = "=";
                if(fileStream.get(ch)) {
                    // checking for `==` for `if` conditions`
                    if(ch == '=') {
                        token.text += ch;
                        token.type = TOKEN_EQUAL;
                        return token;
                    }
                }
                token.type = TOKEN_ASSIGN;
                return token;
            case '+':
                token.text = "+";
                token.type = TOKEN_ADD;
                return token;
            case '-':
                token.text = "-";
                token.type = TOKEN_SUBTRACT;
                return token;
            case '(':
                token.text = "(";
                token.type = TOKEN_LPAREN;
                return token;
            case ')':
                token.text = ")";
                token.type = TOKEN_RPAREN;
                return token;
            case ';':
                token.text = ";";
                token.type = TOKEN_SEMICOLON;
                return token;
            default:
                break;
        }
    }
    token.text = "";
    token.type = TOKEN_EOF;
    return token;
}

std::vector<Token> tokenizeFile(const std::string filePath) {
    std::ifstream file;
    file.open(filePath);

    // check for error during file read
    if(!file.is_open()) {
        perror("Error: Could not open file\n");
        
        if(file.bad()) {
            perror("Fatal error: Badbit is set\n");
        }

        if(file.fail()) {
            std::cerr << "Error: " << strerror(errno) << std::endl;
        }
    }

    std::vector<Token> programTokens;
    Token token;
    do {
        token = getNextToken(file);
        programTokens.push_back(token);
    } while(token.type != TOKEN_EOF);

    return programTokens;
}

