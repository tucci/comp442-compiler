#include "stdafx.h"


Compiler::Compiler() {
	// Create the spec for the tokens
	spec = std::shared_ptr<Specification>(new Specification());
	// Read the grammar from the file that our parser will use
	grammar = std::shared_ptr<Grammar>(new Grammar("grammar.txt", "prog"));
	// Create the lexer with the given specification
	lexer = std::shared_ptr<Lexer>(new Lexer(spec.get()));
	// Create the parser given from our lexer and grammar
	parser = std::shared_ptr<Parser>(new Parser(lexer.get(), grammar.get()));
	parser->outputParserDataToFile();
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
	bool parsed = parser->parse();
	parser->outputAnalysis();
	return parsed;
}


