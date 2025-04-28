# 🐚 Minishell

A lightweight shell implementation focusing on lexical analysis and parsing with Abstract Syntax Tree (AST) generation.

## 📋 Project Overview

Minishell is an educational project that recreates the basic functionality of a Unix shell. This implementation specifically focuses on the lexing and parsing components, creating a robust foundation for command interpretation.

## ✨ Features

### Lexical Analysis
- Tokenization of shell commands
- Support for quotes (single and double)
- Recognition of redirections (<, >, <<, >>)
- Pipe operator handling
- Environment variable identification ($VAR, $?)

### Parser & AST
- Abstract Syntax Tree generation
- Command and argument parsing
- Support for I/O redirections
- Pipeline command structure
- Environment variable expansion
- Error detection and reporting

## 🛠️ Technical Stack

- Language: C
- Build System: Make
- Memory Management: Custom implementation
- Data Structures: AST, Token Lists, Symbol Tables

## 🚀 Getting Started

### Prerequisites
- GCC compiler
- Make
- Unix-like operating system

### Building the Project

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
make
```

### Running Minishell

```bash
./minishell
```

## 📝 Implementation Details

### Lexer Components
- **Token Recognition**: Identifies words, operators, and special characters
- **Quote Handler**: Processes quoted strings with appropriate rules
- **Variable Detector**: Identifies environment variables for expansion

### Parser Design
- **Recursive Descent Parser**: Top-down parsing approach
- **AST Builder**: Creates a structured representation of commands
- **Grammar Rules**: Implements shell grammar specification
- **Error Handler**: Provides meaningful syntax error messages

## 🧪 Test Suite

The project includes comprehensive test cases covering:
- Basic command parsing
- Complex quoting scenarios
- Redirection combinations
- Pipeline structures
- Error detection

Run tests with:
```bash
make test
```

## 📊 Architecture

```
minishell/
├── includes/        # Header files
│   ├── lexer.h      # Lexical analyzer definitions
│   ├── parser.h     # Parser interface
│   └── ast.h        # Abstract Syntax Tree structures
├── srcs/            # Source code
│   ├── lexer/       # Tokenization implementation
│   ├── parser/      # Parsing logic
│   └── ast/         # AST generation and management
├── tests/           # Test suite
└── Makefile         # Build configuration
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🔍 Key Concepts

- **Tokenization**: Breaking input string into meaningful tokens
- **Abstract Syntax Tree**: Hierarchical representation of command structure
- **Grammar Rules**: Formal language specification for shell commands
- **Recursive Descent Parsing**: Top-down parsing technique
- **Symbol Resolution**: Handling variable references and expansions

## 📚 Resources

For those interested in learning more about shell implementation:
- [Write a Shell in C](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- [Let's Build a Shell](https://github.com/kamalmarhubi/shell-workshop)
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)

## 🌟 Acknowledgements

This project is part of the 42 School curriculum, focusing on the implementation of lexical analysis and parsing components of a shell interpreter.