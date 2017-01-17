#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	non_token_t,// special token to tell the tokenizer that this is not a token
	non_zero_t
};

struct token {
	std::string lexeme;
	token_type type;
};

#endif // !TOKEN_H