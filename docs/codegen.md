# Code Generation

The code generation translates the Abstract Syntax Tree (AST) produced by the parser into assembly code for an 8-bit CPU. Focuses on converting high-level constructs into low-level instructions, which can then be executed by a CPU simulator. Below is an explanation of the code generation process, including the intermediate steps and the assembly output format.

## Assembly structure

The generated assembly is organized into two main sections:

**1. Data Section**

Declares memory locations for all variables used in the program.

```asm
.data
x = 255
y = 256
```

**2. Text section**

 Contains the executable instructions.

 ```asm
 .text
ldi A 10
mov M A %x
hlt
```

## Codegen rules

The translation from the AST to assembly code follows specific rules for each language construct. The grammar constructs and their corresponding assembly generation are as follows:


`<program> ::= <statement_list>`
   - Traverse all statements and generate assembly for each.


`<statement_list> ::= <statement> <statement_list> | E`
   - Recursively handle statements in the list.
   - Empty list (`E`) generates no assembly.

`<statement> ::= <declaration> | <assignment> | <conditional>`
   - Delegate to appropriate handlers for declarations, assignments, or conditionals.

`<declaration> ::= "int" <identifier> ";"`
   - Add the variable to the `.data` section.
   - Example: `"int x;"` generates `.data x = [default_value]`.

`<assignment> ::= <identifier> "=" <expression> ";"`
   - Evaluate the expression and store the result in the variable.
   - Example: `"x = 10;"` generates:
     ```assembly
     ldi A 10
     mov M A %x
     ```

`<conditional> ::= "if" "(" <condition> ")" "{" <statement_list> "}`
   - Generate assembly for the condition and statements inside the block.
   - Add branching logic for the `if` condition.
   - Example: 
     ```c
     if (x == 10) {
        y = 20;
     }
     ```
     Generates:
     ```assembly
     ldi A 10
     mov B M %x
     cmp A B
     jne end_if
     ldi A 20
     mov M A %y
     end_if:
     ```

`<expression> ::= <term> <expression_tail>`
   - Evaluate the term and combine it with additional terms as specified in the expression tail.

```
<expression_tail> ::= "+" <term> <expression_tail> | "-" <term> <expression_tail> | E
```
   - Handle addition and subtraction recursively.

`<condition> ::= <expression> "==" <expression>`
   - Generate assembly to compare two expressions.
   - Example: `"x == 10"` generates:
     ```assembly
     ldi A 10
     mov B M %x
     cmp A B
     ```

`<term> ::= <identifier> | <number>`
   - Load the value of a variable or a constant into a register.

---

## Error Handling

The code generation module validates input during translation:
- **Undefined Variables**: Errors are raised if a variable is used without being declared.
- **Unsupported Operations**: Operations outside of addition, subtraction, or equality comparisons are flagged.
- **Missing Components**: Missing parts of an AST node (e.g., incomplete conditions) result in an error.

---

## Limitations
- The module currently supports:
  - Only addition and subtraction for expressions.
  - Equality (`==`) for conditions in `if` statements.
  - Integer variables (`int`) and literals.
- `else` clauses, complex conditions, and other data types are not supported.

---

## Example Workflow

### Input Source Code:
```c
int x;
x = 10;
if (x == 10) {
    int y;
    y = 20;
}
```

Generated assembly

```assembly
.data
x = 255
y = 256

.text
ldi A 10
mov M A %x
ldi A 10
mov B M %x
cmp A B
jne end_if
ldi A 20
mov M A %y
end_if:
hlt
```
