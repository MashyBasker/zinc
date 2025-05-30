# Zinc Compiler

This repository contains the implementation of a compiler for Zinc, a custom-designed programming language. The compiler translates Zinc code into assembly code for an 8-bit CPU.

## Project structure

`src` folder contains the implementation of the Lexer, Parser and Code Generator

`lexer.cpp` -> Implements the Lexical analysis and converting the file into a token stream \
`lexer.h` -> Header file for the lexer \
`parser.cpp` -> Implements the parser to convert the token stream into an AST\
`parser.h` -> Header file for the parser \
`codegen.cpp` ->  Implements the code generation to traverse the AST and map to the assembly instructions of the 8-bit computer\
`codegen.h` -> Header file for the code generation

The `data` folder contains the sample Simple Lang programs that can be used to test the compiler


## Quickstart

**Build the project**
```bash
make
```

**Compile a sample program**

```bash
./simp /path/to/program
```

The assembly is generated to a `.asm` file in the same folder as the source code.

> Note: As of now there is no assembler for the generated assembly so we will leverage the assembler provided by the 8-bit computer

**Clone the 8-bit computer**

```bash
git clone https://github.com/lightcode/8bit-computer
```

**Build the computer**
```bash
make build
```

**Assemble the generated simple lang assembly**

```bash
./asm/asm.py /path/to/assembly > memory.list
```

**Run the 8-bit computer on the program**

```bash
./computer
```
