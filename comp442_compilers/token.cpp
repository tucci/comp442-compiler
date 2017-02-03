#include "stdafx.h"

// Override for when we are outputting token
std::ostream& operator << (std::ostream& o, Token& token) {
	if (token.type == TokenType::error_token) {
		o << "error: " << token.lexeme << "\t\tmessage: " << token.error.errorMessage << "\t\tline number: " << std::to_string(token.tokenLine) << std::endl;
	} else {
		std::string tokenTypeString = Specification::TOKEN_PRINT_MAP.at(token.type);
		o << "lexeme: " << token.lexeme << "\t\ttype: " << tokenTypeString << "\t\tline number: " << std::to_string(token.tokenLine) << std::endl;
	}
	return o;
};