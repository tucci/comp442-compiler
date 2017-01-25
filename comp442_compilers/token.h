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
	cmt,			// comment
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


// Simple data structure to hold the data about our token
struct Token {
	// the lexeme for this token
	std::string lexeme;
	// the token type for this token. This refers to the enum value
	TokenType type;
	// the line in the source file this token can be found
	int tokenLine; 
};

#endif // !TOKEN_H