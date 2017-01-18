#ifndef LEXER_H
#define LEXER_H

#include "specification.h"

class lexer {
public:
	// Create the lexer with the specifiec language specification for this lexer to use
	lexer(specification spec);
	~lexer();
	// Spits out the next token that the lexer reads
	token next_token();
	void set_source(std::string path_to_file);

private:
	std::ifstream file;
	// The state transiton table tokenizer that holds all the rules for this lexer
	dfa spec;
	// Returns the next char in the stream for the tokenizer to read from
	std::string next_char();

	std::string source_file;
	int current_line_index;
	int current_char_index;
};

#endif

