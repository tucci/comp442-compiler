#ifndef LEXER_H
#define LEXER_H


class lexer {
public:
	lexer();
	~lexer();
	// Spits out the next token that the lexer reads
	token next_token();

private:
	std::string source;
	int index;
	// The state transiton table tokenizer that holds all the rules for this lexer
	dfa tokenizer;
	// Returns the next char in the stream for the tokenizer to read from
	std::string next_char();
	// Creates and setups up the rules for our tokenizer in order for our lexer to work
	void init_tokenizer();
};

#endif

