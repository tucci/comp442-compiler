#ifndef LEXER_H
#define LEXER_H


class lexer {
public:
	lexer();
	~lexer();
	std::string next_token();

private:
	std::string source;
	int index;
	dfa tokenizer;
	std::string next_char();
	void init_tokenizer();
};

#endif

