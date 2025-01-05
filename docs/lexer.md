# Lexer/Tokenizer

The tokens for the Simple Lang constructs will be

```
TOKEN_IDENTIFIER
TOKEN_INT
TOKEN_NUMBER
TOKEN_ASSIGN
TOKEN_ADD
TOKEN_SUBTRACT
TOKEN_IF
TOKEN_EQUAL
TOKEN_LPAREN
TOKEN_RPAREN
TOKEN_LBRACE
TOKEN_RBRACE
TOKEN_SEMICOLON
TOKEN_EOF
```

- These tokens will be defined in a `TokType` enum.
- The tokens are defined by a `Token` struct which contains the lexeme `string` and the type of token, from the enum.
- The function for getting the next token, creates a vector of tokens -> `vector<Token>` after making a single pass of the file.