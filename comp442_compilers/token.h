#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	// Special tokens
	non_token,		// this means this state is an intermediate state
					// special token to tell the lexer that this is not a token and should carry on
	error_token,	// error token, specifiying that there was no token found

	// Tokens part of our language
	id,				// identifier token
	num,			// Number token
	reserved_word,	// Used for reserved words in the language
	andlog,			// logical and operator
	orlog,			// logical or operator
	notlog,			// logical not operator
	dot,			// . dot operator
	comma,			// , comma operator
	lt,				// < less than operator
	lesseq,			// <= less than or equal operator
	noteq,			// <> not equal to operator
	gt,				// > greater than operator
	greateq,		// >= greater than or equal operator
	assgn,			// = assignment operator
	comparison,		// == comparison operator
	adddop,			// + addition operator
	subtractop,		// - subtraction operator
	multop,			// * multiplication operator
	divop,			// / division operator
	openpar,		// ( open paren
	closepar,		// ) close paren
	openbrace,		// { open brace
	closebrace,		// } close brace
	opensquare,		// [ open square
	closesquare,	// ] close square
	semicolon,		// ; semi colon, or end of statement
};

struct token {
	// the lexeme for this token
	std::string lexeme;
	// the token type for this token.
	token_type type;
	int token_line; // the line in the source file this token can be found
	int token_column;// the character position in that line this token can be found
	// Question: Do these locations disregard empty lines and line comments?
	// Answer: No. Lines with comments are not disregared. It shows the exact location in the source file
};

#endif // !TOKEN_H