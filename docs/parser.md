# Parser

The tokens returned by the tokenizer as a `vector<Token>` will be used by the parser to generate an Abstract Syntax Tree. Based on the language constructs provided, the BNF of the grammar will look like this.

E is the epsilon i.e represents a null.

```markdown
<program> ::= <statement_list>
<statement_list> ::= <statement> <statement_list> | E
<statement> ::= <declaration> | <assignment> | <conditional>
<declaration> ::= "int" <identifier> ";" /// only integers are considered for now
<assignment> ::= <identifier> "=" <expression> ";"
<conditional> ::= "if" "(" <condition> ")" "{" <statement_list> "}
<expression> ::= <term> <expression_tail>
<term> ::= <identifier> | <number>
<expression_tail> ::= "+" <term> <expression_tail> | "-" <term> <expression> /// only addition and subtraction is supported for now
<condition> ::= <expression> "==" <expression>
<identifier> ::= <letter> <identifier_tail>
<number> ::= <digit> <number_tail>
<identifier_tail> ::= <letter> <identifier_tail> | E
<number_tail> ::= <digit> <number_tail> | E
<letter> ::= "a" | "b" ... "z" | "A" | "B" ... "Z"
<number> ::= "0" | "1" | "2" ... "9"
```

There is no left recursion in this grammar, so we will use a recursive descent parser.
A left recursion is a grammar which looks like this:

$T \rightarrow T V | d$

This causes an infinite loop.

int a;
int b;
int c;
int d;
a = 3;
b = 4;
c = 0;
d = 44;