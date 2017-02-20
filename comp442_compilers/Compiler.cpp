#include "stdafx.h"


Compiler::Compiler() {
	// Create the spec for the tokens
	spec = std::unique_ptr<Specification>(new Specification());
	// Read the grammar from the file that our parser will use
	grammar = std::unique_ptr<Grammar>(new Grammar("grammar.txt", "prog"));
	// Create the lexer with the given specification
	lexer = std::unique_ptr<Lexer>(new Lexer(spec.get()));
	// Create the parser given from our lexer and grammar
	parser = std::unique_ptr<Parser>(new Parser(*lexer.get(), *grammar.get()));
}


Compiler::~Compiler() {
}

// Compiles our source code the to the target code
void Compiler::compile() {
	// Method stub
}

// Sets the source file for our compile to compile
void Compiler::setSourceFile(std::string sourceFile) {
	lexer->setSource(sourceFile);
}

// Analyses the syntax of the source code
bool Compiler::analyzeSyntax() {
	return parser->parse();
}


