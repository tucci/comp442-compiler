#ifndef TOKEN_H
#define TOKEN_H

enum TokenType {
	// Special tokens
	non_token,		// this means this state is an intermediate state
					// special token to tell the lexer that this is not a token and should carry on
	error_token,	// error token, specifiying that there was no token found
	// Tokens part of our language
	id,				// identifier token
	num,			// Number token
	int_value,		// integer token that holds values. Not the int keyword
	float_value,	// float token that holds values. Not the float keyword
	and,			// logical and operator
	or,				// logical or operator
	not,			// logical not operator
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
	cmt,			// comment, these will be stripped off by the lexer. so there will never comment tokens given to the user
	cmt_line_start,	// special token to indicate we are at the start of a comment used by lexer
	cmt_multi_start,// special token to indicate we are at the start of a comment used by lexer
	// Reserved word tokens
	if_token,		// if keyword
	then_token,		// then keyword
	else_token,		// else keyword
	for_token,		// for keyword
	class_token,	// class keyword
	int_token,		// int keyword
	float_token,	// float keyword
	get_token,		// get keyword
	put_token,		// put keyword
	return_token,	// return keyword
	program_token,	// program keyword
};

// A simple list of types of errors that can occur while scanning the source code
enum ErrorType {
	unknown_symbol, // Error for symbols our tokenizer doesnt understand
	invalid_float, // Error for when floats don't follow the specification. I.e 1.00
	incomplete_float, // Error for floats that are in form 3.a. This is not an error, but it is a flag to tell the lexer to recover from this
};

// A simple data structure holding the error message and type
struct TokenError {
	std::string errorMessage;
	ErrorType type;
};


// Simple data structure to hold the data about our token
struct Token {
	// the lexeme for this token
	std::string lexeme;
	// the token type for this token. This refers to the enum value
	TokenType type;
	// the line in the source file this token can be found
	int tokenLine;
	// The error data structure in case this token is an error token
	TokenError error;
	// Override output for a token
	friend std::ostream& operator << (std::ostream& o, Token& token);
};

#endif // !TOKEN_H