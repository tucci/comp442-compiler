#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	// special token for the lexer to tell that this is a nullable value
	null_token,
	// special token to tell the lexer that this is not a token and should carry on
	// this means this state is an intermediate state
	non_token_t,
	non_zero_t,
	id,
	num,
};

struct token {
	// the lexeme for this token
	std::string lexeme;
	// the token type for this token.
	token_type type;
	int token_line; // the line in the source file this token can be found
	int token_location;// the character position in that line this token can be found
	// Question: Do these locations disregard empty lines and line comments?
	// Answer: No. Lines with comments are not disregared. It shows the exact location in the source file
};

#endif // !TOKEN_H