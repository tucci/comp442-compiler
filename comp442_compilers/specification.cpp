#include "stdafx.h"

// A set of all the operartors for the language
const std::unordered_map<std::string, TokenType> Specification::TOKEN_MAP = {
	// Listed in the order from kfg edit
	{ "class", TokenType::class_token },
	{ "id", TokenType::id },
	{ "{", TokenType::openbrace },
	{ "}", TokenType::closebrace },
	{ ";", TokenType::semicolon },
	{ "program", TokenType::program_token },
	{ "(", TokenType::openpar },
	{ ")", TokenType::closepar },
	{ "for", TokenType::for_token },
	{ "if", TokenType::if_token },
	{ "then", TokenType::then_token },
	{ "get", TokenType::get_token },
	{ "put", TokenType::put_token },
	{ "return", TokenType::return_token },
	{ "else", TokenType::else_token },
	{ ".", TokenType::dot },
	{ "+", TokenType::addop },
	{ "-", TokenType::subtractop },
	{ "num", TokenType::num },
	{ "not", TokenType::not },
	{ "[", TokenType::opensquare },
	{ "]", TokenType::closesquare },
	{ "integer", TokenType::int_value },
	{ "float", TokenType::float_token },
	{ "int", TokenType::int_token },
	{ ",", TokenType::comma },
	{ "=", TokenType::assgn },
	{ "<", TokenType::lt },
	{ ">", TokenType::gt },
	{ "or", TokenType:: or },
	{ "*", TokenType::multop },
	{ "/", TokenType::divop },
	{ "and", TokenType::and },
	{ "$", TokenType::end_of_file_token }, // TODO: when calling next token, output a end of file token when done
	{ "<=", TokenType::lesseq },
	{ "<>", TokenType::noteq },
	{ ">=", TokenType::greateq },
	{ "==", TokenType::comparison},
	
};

const std::unordered_map<TokenType, std::string> Specification::REVERSE_TOKEN_MAP = constructReverseMap();

std::unordered_map<TokenType, std::string> Specification::constructReverseMap() {
	std::unordered_map<TokenType, std::string> reverseMap;
	for (const auto& it : TOKEN_MAP) {
		reverseMap.emplace(it.second, it.first);
	}
	return reverseMap;
};

const std::unordered_map<TokenType, std::string> Specification::TOKEN_PRINT_MAP = {
	{ TokenType::non_token, "non_token" },
	{ TokenType::error_token, "error_token" },
	{ TokenType::id, "id" },
	{ TokenType::num, "num_value" },
	{ TokenType::int_value, "int_value" },
	{ TokenType::float_value, "float_value" },
	{ TokenType::and, "and"},
	{ TokenType::or, "or" },
	{ TokenType::not, "not" },
	{ TokenType::dot, "dot" },
	{ TokenType::comma, "comma" },
	{ TokenType::lt, "lt" },
	{ TokenType::lesseq, "lesseq" },
	{ TokenType::noteq, "noteq" },
	{ TokenType::gt, "gt" },
	{ TokenType::greateq, "greateq" },
	{ TokenType::assgn, "assgn" },
	{ TokenType::comparison, "comparison" },
	{ TokenType::addop, "addop" },
	{ TokenType::subtractop, "subtractop" },
	{ TokenType::multop, "multop"},
	{ TokenType::divop, "divop" },
	{ TokenType::openpar, "openpar" },
	{ TokenType::closepar, "closepar"},
	{ TokenType::openbrace, "openbrace" },
	{ TokenType::closebrace, "closebrace" },
	{ TokenType::opensquare, "opensquare" },
	{ TokenType::closesquare, "closesquare" },
	{ TokenType::semicolon, "semicolon" },
	{ TokenType::if_token, "if_token" },
	{ TokenType::then_token, "then_token" },
	{ TokenType::else_token, "else_token" },
	{ TokenType::for_token, "for_token"  },
	{ TokenType::class_token, "class_token" },
	{ TokenType::int_token, "int_token" },
	{ TokenType::float_token, "float_token" },
	{ TokenType::get_token, "get_token" },
	{ TokenType::put_token, "put_token" },
	{ TokenType::return_token, "return_token" },
	{ TokenType::program_token, "program_token" }
};

Specification::Specification(bool useDefault) {
	// Here we create the lanugage elements and specs into the dfa
	// Create empty dfa
	spec = std::shared_ptr<Dfa>(new Dfa);

	// If we use the default, it will create the spec accoring to what was provided in the assignment
	// If false, then the user can create their own spec by hand. This is used for testing purposes
	if (useDefault) {

		State* start = spec->createStartState();

		// Do not make the error state be a backup state
		State* error = spec->createState(true, false, TokenType::error_token);
		error->errorType = ErrorType::unknown_symbol;
		// Error state
		spec->addElseTransition(start, error);

		// Add white space transitions
		whitespaceTransitions(start, start);

		// id state transitions
		State* idIntermediate = spec->createState();
		State* idState = spec->createState(true, true, TokenType::id);
		Specification::idTransitions(start, idIntermediate);
		spec->addElseTransition(idIntermediate, idState);

		// num state transitions
		State* numIntermediate1 = spec->createState();
		State* numIntermediate2 = spec->createState();
		State* numIntermediate3 = spec->createState();
		State* numState = spec->createState(true, true, TokenType::num);
		Specification::numTransitions(start, numIntermediate1, numIntermediate2, numIntermediate3);
		spec->addElseTransition(numIntermediate1, numState);
		spec->addElseTransition(numIntermediate2, numState);
		spec->addElseTransition(numIntermediate3, numState);
		// Quick hack to get internal intermediate state for the fractions
		State* fractionIntermediate1 = spec->table(numIntermediate1->stateIdentifier, ".");
		State* fractionIntermediate2 = spec->table(numIntermediate2->stateIdentifier, ".");
		State* fractionIntermediate3 = spec->table(spec->table(fractionIntermediate2->stateIdentifier, "0")->stateIdentifier, "0");
		State* errorStateForFloat = spec->createState(true, false, TokenType::error_token);
		State* errorStateForFloat2 = spec->createState(true, false, TokenType::error_token);
		errorStateForFloat->errorType = ErrorType::incomplete_float;
		errorStateForFloat2->errorType = ErrorType::invalid_float;
		// Attach else error states
		spec->addElseTransition(fractionIntermediate1, errorStateForFloat);
		spec->addElseTransition(fractionIntermediate2, errorStateForFloat);
		spec->addElseTransition(fractionIntermediate3, errorStateForFloat2);
		// End hack

		// Operators

		// . dot token
		State* dotIntermediate = spec->createState();
		spec->addTransition(start, ".", dotIntermediate);
		State* dotState = spec->createState(true, true, TokenType::dot);
		spec->addElseTransition(dotIntermediate, dotState);

		// , comma token
		State* commaIntermediate = spec->createState();
		spec->addTransition(start, ",", commaIntermediate);
		State* commaState = spec->createState(true, true, TokenType::comma);
		spec->addElseTransition(commaIntermediate, commaState);

		// <, <=, <>
		State* ltIntermediate = spec->createState();
		spec->addTransition(start, "<", ltIntermediate);
		// < less than token
		State* ltState = spec->createState(true, true, TokenType::lt);
		spec->addElseTransition(ltIntermediate, ltState);
		// <= less than or equal token
		State* lesseqState = spec->createState(true, false, TokenType::lesseq);
		spec->addTransition(ltIntermediate, "=", lesseqState);
		// <> not equal token
		State* noteqState = spec->createState(true, false, TokenType::noteq);
		spec->addTransition(ltIntermediate, ">", noteqState);

		// >, >=
		State* gtIntermediate = spec->createState();
		spec->addTransition(start, ">", gtIntermediate);
		// > greater than token
		State* gtState = spec->createState(true, true, TokenType::gt);
		spec->addElseTransition(gtIntermediate, gtState);
		// >= greater than or equal token
		State* greateqState = spec->createState(true, false, TokenType::greateq);
		spec->addTransition(gtIntermediate, "=", greateqState);

		// =, ==
		State* eqIntermediate = spec->createState();
		spec->addTransition(start, "=", eqIntermediate);
		// = assignment token
		State* assgnState = spec->createState(true, true, TokenType::assgn);
		spec->addElseTransition(eqIntermediate, assgnState);
		// == comparison token
		State* comparisonState = spec->createState(true, false, TokenType::comparison);
		spec->addTransition(eqIntermediate, "=", comparisonState);

		// +, -, *, /
		// + addition operator token
		State* addIntermediate = spec->createState();
		spec->addTransition(start, "+", addIntermediate);
		State* addState = spec->createState(true, true, TokenType::addop);
		spec->addElseTransition(addIntermediate, addState);
		// - subrtaction operator token
		State* subtractIntermediate = spec->createState();
		spec->addTransition(start, "-", subtractIntermediate);
		State* subtractState = spec->createState(true, true, TokenType::subtractop);
		spec->addElseTransition(subtractIntermediate, subtractState);
		// multiplication operator token
		State* multIntermediate = spec->createState();
		spec->addTransition(start, "*", multIntermediate);
		State* multState = spec->createState(true, true, TokenType::multop);
		spec->addElseTransition(multIntermediate, multState);
		// division operator token
		State* divIntermediate = spec->createState();
		spec->addTransition(start, "/", divIntermediate);
		State* divState = spec->createState(true, true, TokenType::divop);
		spec->addElseTransition(divIntermediate, divState);

		// Comments
		// multi line comment tokens
		State* multiCmtIntermediate1 = spec->createState(false, false, TokenType::cmt_multi_start);
		spec->addTransition(divIntermediate, "*", multiCmtIntermediate1);
		spec->addElseTransition(multiCmtIntermediate1, multiCmtIntermediate1);
		State* multiCmtIntermediate2 = spec->createState();
		spec->addTransition(multiCmtIntermediate1, "*", multiCmtIntermediate2);
		spec->addElseTransition(multiCmtIntermediate2, multiCmtIntermediate1);
		spec->addTransition(multiCmtIntermediate2, "*", multiCmtIntermediate2);
		State* multiCmt = spec->createState(true, false, TokenType::cmt);
		spec->addTransition(multiCmtIntermediate2, "/", multiCmt);
		// Comments that dont close error
		State* multicmtNoCloseError = spec->createState(false, false, TokenType::error_token);
		multicmtNoCloseError->errorType = ErrorType::mulitcomment_error;
		spec->addTransition(multiCmtIntermediate1, std::to_string(EOF), multicmtNoCloseError);
		spec->addTransition(multiCmtIntermediate2, std::to_string(EOF), multicmtNoCloseError);
		spec->addTransition(multiCmt, std::to_string(EOF), multicmtNoCloseError);

		// Single line comment
		State* lineCmtIntermediate = spec->createState(false, false, TokenType::cmt_line_start);
		spec->addTransition(divIntermediate, "/", lineCmtIntermediate);
		State* lineCmt = spec->createState(true, true, TokenType::cmt);
		spec->addElseTransition(lineCmtIntermediate, lineCmtIntermediate);
		spec->addTransition(lineCmtIntermediate, "\n", lineCmt);
		spec->addTransition(lineCmtIntermediate, std::to_string(EOF), lineCmt);

		// Parenthensis, brackets, braces
		// ( open paren token
		State* openparIntermediate = spec->createState();
		spec->addTransition(start, "(", openparIntermediate);
		State* openparState = spec->createState(true, true, TokenType::openpar);
		spec->addElseTransition(openparIntermediate, openparState);
		// ) close paren token
		State* closeparIntermediate = spec->createState();
		spec->addTransition(start, ")", closeparIntermediate);
		State* closeparState = spec->createState(true, true, TokenType::closepar);
		spec->addElseTransition(closeparIntermediate, closeparState);
		// { open brace token
		State* openbraceIntermediate = spec->createState();
		spec->addTransition(start, "{", openbraceIntermediate);
		State* openbraceState = spec->createState(true, true, TokenType::openbrace);
		spec->addElseTransition(openbraceIntermediate, openbraceState);
		// } close brace token
		State* closebraceIntermediate = spec->createState();
		spec->addTransition(start, "}", closebraceIntermediate);
		State* closebraceState = spec->createState(true, true, TokenType::closebrace);
		spec->addElseTransition(closebraceIntermediate, closebraceState);
		// [ open square token
		State* opensquareIntermediate = spec->createState();
		spec->addTransition(start, "[", opensquareIntermediate);
		State* opensquareState = spec->createState(true, true, TokenType::opensquare);
		spec->addElseTransition(opensquareIntermediate, opensquareState);
		// ] close square token
		State* closesquareIntermediate = spec->createState();
		spec->addTransition(start, "]", closesquareIntermediate);
		State* closesquareState = spec->createState(true, true, TokenType::closesquare);
		spec->addElseTransition(closesquareIntermediate, closesquareState);

		// ; semi colon token
		State* semicolonIntermediate = spec->createState();
		spec->addTransition(start, ";", semicolonIntermediate);
		State* semicolonState = spec->createState(true, true, TokenType::semicolon);
		spec->addElseTransition(semicolonIntermediate, semicolonState);
	}
	
}

Specification::~Specification() {
}

std::shared_ptr<Dfa> Specification::getSpec() {
	return spec;
}

TokenType Specification::getTokenTypeForInput(std::string input) {
	Token token = {input,  spec->stateFromInput(input)->tokenType , 0};
	updateTokenType(&token);
	return token.type;
}

void Specification::updateTokenType(Token* token) {
	if (token->type == TokenType::id) {
		// Check to see if this identifier is in our token map
		std::unordered_map<std::string, TokenType>::const_iterator inTokenMap = Specification::TOKEN_MAP.find(token->lexeme);
		// We will change it from an identifier token to it's respective token
		if (inTokenMap != Specification::TOKEN_MAP.end()) {
			token->type = inTokenMap->second;
		}
	}
	// Convert num types to their respective int/float tokens
	if (token->type == TokenType::num) {
		// if it is a float we will change it
		if (token->lexeme.find('.') != std::string::npos) {
			token->type = TokenType::float_value;
		} else {
			// if it is an integer we will change it to that
			token->type = TokenType::int_value;
		}
	}
}


void Specification::whitespaceTransitions(State* start, State* end) {
	spec->addTransition(start, " ", end);
	spec->addTransition(start, "\t", end);
	spec->addTransition(start, "\n", end);
	spec->addTransition(start, "\v", end);
	spec->addTransition(start, "\r", end);
	spec->addTransition(start, "\f", end);
	spec->addTransition(start, "\r\n", end);
}

void Specification::nonzeroTransitions(State* start, State* end) {
	std::string rule = "123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions 1 - 9 from the start state and end state
		// this will create 9 transitions
		spec->addTransition(start, rule.substr(i, 1), end);
	}
}

void Specification::digitTransitions(State* start, State* end) {
	std::string rule = "0123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions 0 - 9 from the start state and end state
		// this will create 10 transitions
		spec->addTransition(start, rule.substr(i, 1), end);
	}
}

void Specification::letterTransitions(State* start, State* end) {
	std::string rule = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions a - z and A - Z from the start state and end state
		// this will create 26 * 2 transitions
		spec->addTransition(start, rule.substr(i, 1), end);
	}
}

void Specification::floatTransitions(State* start, State* end) {
	State* intermediate1 = spec->createState();
	State* intermediate2 = spec->createState();
	integerTransitions(start, intermediate1, intermediate2);
	fractionTransitions(intermediate1, end);
	fractionTransitions(intermediate2, end);
}

void Specification::fractionTransitions(State* start, State* end) {
	State* intermediate1 = spec->createState();
	spec->addTransition(start, ".", intermediate1);
	digitTransitions(intermediate1, end);
	// if all the end transitions have already been created. we dont want to recreate them
	// we will just link the new intermediate to the same end
	// This can happen when there are two fraction tranistions coming out of two states
	// this is to avoid duplicate intermediate states
	if (spec->table(end->stateIdentifier, "0") == NULL) {
		nonzeroTransitions(end, end);
		State* intermediate2 = spec->createState();
		spec->addTransition(end, "0", intermediate2);
		spec->addTransition(intermediate2, "0", intermediate2);
		nonzeroTransitions(intermediate2, end);
	}
}

void Specification::integerTransitions(State* start, State* end, State* end2) {
	spec->addTransition(start, "0", end2);
	nonzeroTransitions(start, end);
	digitTransitions(end, end);
}

void Specification::numTransitions(State* start, State* end, State* end2, State* end3) {
	nonzeroTransitions(start, end);
	digitTransitions(end, end);
	spec->addTransition(start, "0", end2);
	// Question: will this create duplicate inner states to be created
	// Answer: No. this is handled internally.
	fractionTransitions(end, end3);
	fractionTransitions(end2, end3);
}

void Specification::alphanumTransitions(State* start, State* end) {
	spec->addTransition(start, "_", end);
	letterTransitions(start, end);
	digitTransitions(start, end);
}

void Specification::idTransitions(State* start, State* end) {
	letterTransitions(start, end);
	alphanumTransitions(end, end);
}
