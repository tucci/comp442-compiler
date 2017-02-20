#include "stdafx.h"


Compiler::Compiler() {
	spec = std::unique_ptr<Specification>(new Specification());
	grammar = std::unique_ptr<Grammar>(new Grammar("grammar.txt", "prog"));
	lexer = std::unique_ptr<Lexer>(new Lexer(spec.get()));
	parser = std::unique_ptr<Parser>(new Parser(*lexer.get(), *grammar.get()));
}


Compiler::~Compiler() {
}

void Compiler::compile() {
	// Method stub
}

void Compiler::setSourceFile(std::string sourceFile) {
	lexer->setSource(sourceFile);
}

bool Compiler::checkSyntax() {
	return parser->parse();
}


