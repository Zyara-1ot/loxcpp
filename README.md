## LoxCpp - A Tree-Walking Interpreter in C++

A clean, from-scratch implementation of the Lox programming language interpreter built in C++ to deeply understand how interpreters work.
This project is inspired by the language from Crafting Interpreters and built with clarity in mind for learning, experimentation.

## What is This?

LoxCpp is a **tree-walk interpreter** that executes Lox programs by walking through an Abstract Syntax Tree. It implements the complete interpretation pipeline:
```
Source Code → Scanner → Tokens → Parser → AST → Interpreter → Result
```
It currently supports:

> Numeric and string expressions

> Unary , binary , literal and grouping operations

> A REPL (interactive prompt)

> runtime error handling

Built following the excellent [Crafting Interpreters](https://craftinginterpreters.com/) book, but implemented in C++ instead of Java .

##  Why I Built This

I wanted to understand how programming languages actually work , not just use them, but understand them from the ground up. This project taught me:

- How scanners tokenize source code character by character
- How recursive descent parsers construct Abstract Syntax Trees while enforcing operator precedence and associativity rules.
- How tree-walking interpreters evaluate expressions and statements
- The intricacies of scope, closures, and environment chaining
- Memory is managed using C++ smart pointers (`std::shared_ptr`) leveraging RAII and reference counting for deterministic object lifetime management.

## Features

- **Expressions** - Arithmetic, comparison, logical operations
- **Variables** - Declaration, assignment, scoping
- **Control Flow** - If/else statements
- **REPL** - Interactive mode for quick testing

## 🎮 Quick Example
```lox
var x = 10;
{
    var x = 20;
    print x; // 20
}
print x; // 10


## 🛠️ Building & Running

### Build
```bash
# Clone the repository
git clone https://github.com/Zyara-1ot/loxcpp.git
cd loxcpp

# Compile
g++ src/*.cpp -std=c++17 -o lox


```

### Run
```bash
# Run a Lox file
./lox src/test.lox

# Start the REPL
./lox
```

## 📁 Project Structure
```
loxcpp/
├── src/           # Source code (scanner, parser, interpreter, etc.)
├── tests/         # Test files
├── test1.lox      # Example Lox programs
├── testz.lox
└── lox            # Compiled executable
```

## Bugs I Fought Through

Building this taught me that interpreters have bugs at every layer:

**Lexer Issues** - Off-by-one errors caused infinite loops and skipped characters. `10+2` wouldn't split into tokens correctly.

**Precedence Problems** - `1 + 2 * 3` evaluated as `9` instead of `7` until I restructured the parser to encode precedence properly.

**Parser State** - Token pointer management was chaos. Missing `)` crashed instead of reporting errors cleanly.

**AST Construction** - Swapped left/right children. Printed the tree structure to debug.

**Environment Bugs** - Variables not updating, scope behaving wrong, undefined variables crashing the interpreter.

**Evaluation Order** - Expressions evaluated in wrong order. Debugger + print statements saved me.

## 📚 Learning Resources

This project follows [**Crafting Interpreters**](https://craftinginterpreters.com/) by Robert Nystrom - hands down the best book for understanding how to build programming languages from scratch. 

I implemented it in C++ rather than following the book's Java/C to:
- Practice modern C++ (smart pointers, RAII, etc.)
- Understand memory management without garbage collection
- Challenge myself to translate concepts between languages

## 🎯 What I Learned

- **Scanning is harder than it looks** - Edge cases everywhere (string escaping, number parsing, keywords vs identifiers)
- **Recursive descent is elegant** - The parser structure mirrors the grammar beautifully
- **Closures are mind-bending** - Capturing environments properly took several attempts
- **Error messages matter** - Good error reporting makes debugging infinitely easier

## 📜 License

MIT - See [LICENSE](LICENSE) for details

## 🙏 Acknowledgments

- **Robert Nystrom** for [Crafting Interpreters](https://craftinginterpreters.com/) - an incredible resource
- The original **Lox language design**

---

***Built this to learn. Sharing it to help others learn too.*** 
