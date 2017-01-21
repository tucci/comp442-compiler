#include "stdafx.h"


// A set of all the reserved keyword for the language
const std::unordered_set<std::string> specification::RESERVED_WORDS = {
	"if",
	"then",
	"else",
	"for",
	"class",
	"int",
	"float",
	"get",
	"put",
	"return",
	"program"
};

// A set of all the operartors for the language
const std::unordered_map<std::string, token_type> specification::TOKEN_MAP = {
	{ "and", token_type::andlog },
	{ "or", token_type::orlog },
	{ "not", token_type::notlog },
	{ ".", token_type::dot },
	{ ",", token_type::comma },
	{ "<", token_type::lt },
	{ "<=", token_type::lesseq },
	{ "<>", token_type::noteq },
	{ ">", token_type::gt },
	{ ">=", token_type::greateq },
	{ "=", token_type::assgn },
	{ "==", token_type::comparison},
	{ "+", token_type::adddop },
	{ "-", token_type::subtractop },
	{ "*", token_type::multop },
	{ "/", token_type::divop },
	{ "(", token_type::openpar },
	{ ")", token_type::closepar },
	{ "{", token_type::openbrace },
	{ "}", token_type::closebrace },
	{ "[", token_type::opensquare },
	{ "]", token_type::closesquare },
	{ ";", token_type::semicolon },
};

specification::specification() {
	// Here we create the lanugage elements and specs into the dfa
	state* start = spec.create_start_state();

	// Do not make the error state be a backup state
	state* error = spec.create_state(true, false, token_type::error_token);
	// Error state
	spec.add_else_transition(start, error);

	// Add white space transitions
	whitespace_transitions(start, start);
	
	// id state transitions
	state* id_intermediate = spec.create_state();
	state* id_state = spec.create_state(true, true, token_type::id);
	specification::id_transitions(start, id_intermediate);
	spec.add_else_transition(id_intermediate, id_state);

	// num state transitions
	state* num_intermediate1 = spec.create_state();
	state* num_intermediate2 = spec.create_state();
	state* num_intermediate3 = spec.create_state();
	state* num_state = spec.create_state(true, true, token_type::num);
	specification::num_transitions(start, num_intermediate1, num_intermediate2, num_intermediate3);
	spec.add_else_transition(num_intermediate1, num_state);
	spec.add_else_transition(num_intermediate2, num_state);
	spec.add_else_transition(num_intermediate3, num_state);

	// Operators

	// . dot token
	state* dot_intermediate = spec.create_state();
	spec.add_transition(start, ".", dot_intermediate);
	state* dot_state = spec.create_state(true, true, token_type::dot);
	spec.add_else_transition(dot_intermediate, dot_state);

	// , comma token
	state* comma_intermediate = spec.create_state();
	spec.add_transition(start, ",", comma_intermediate);
	state* comma_state = spec.create_state(true, true, token_type::comma);
	spec.add_else_transition(comma_intermediate, comma_state);

	// <, <=, <>
	state* lt_intermediate = spec.create_state();
	spec.add_transition(start, "<", lt_intermediate);
	// < less than token
	state* lt_state = spec.create_state(true, true, token_type::lt);
	spec.add_else_transition(lt_intermediate, lt_state);
	// <= less than or equal token
	state* lesseq_state = spec.create_state(true, false, token_type::lesseq);
	spec.add_transition(lt_intermediate, "=", lesseq_state);
	// <> not equal token
	state* noteq_state = spec.create_state(true, false, token_type::noteq);
	spec.add_transition(lt_intermediate, ">", lesseq_state);

	// >, >=
	state* gt_intermediate = spec.create_state();
	spec.add_transition(start, ">", gt_intermediate);
	// > greater than token
	state* gt_state = spec.create_state(true, true, token_type::gt);
	spec.add_else_transition(gt_intermediate, gt_state);
	// >= greater than or equal token
	state* greateq_state = spec.create_state(true, false, token_type::greateq);
	spec.add_transition(gt_intermediate, "=", greateq_state);

	// =, ==
	state* eq_intermediate = spec.create_state();
	spec.add_transition(start, "=", eq_intermediate);
	// = assignment token
	state* assgn_state = spec.create_state(true, true, token_type::assgn);
	spec.add_else_transition(eq_intermediate, assgn_state);
	// == comparison token
	state* comparison_state = spec.create_state(true, false, token_type::comparison);
	spec.add_transition(eq_intermediate, "=", comparison_state);

	// +, -, *, /
	// + addition operator token
	state* add_intermediate = spec.create_state();
	spec.add_transition(start, "+", add_intermediate);
	state* add_state = spec.create_state(true, true, token_type::adddop);
	spec.add_else_transition(add_intermediate,add_state);
	// - subrtaction operator token
	state* subtract_intermediate = spec.create_state();
	spec.add_transition(start, "-", subtract_intermediate);
	state* subtract_state = spec.create_state(true, true, token_type::subtractop);
	spec.add_else_transition(subtract_intermediate, subtract_state);
	// multiplication operator token
	state* mult_intermediate = spec.create_state();
	spec.add_transition(start, "*", mult_intermediate);
	state* mult_state = spec.create_state(true, true, token_type::multop);
	spec.add_else_transition(mult_intermediate, mult_state);
	// division operator token
	state* div_intermediate = spec.create_state();
	spec.add_transition(start, "/", div_intermediate);
	state* div_state = spec.create_state(true, true, token_type::divop);
	spec.add_else_transition(div_intermediate, div_state);

	// Comments
	// // and /**/ tokens
	// TODO: comments are a little tricky do do nested comments
	// you might have to seperate the /* and */ from each other
	

	// Parenthensis, brackets, braces
	// ( open paren token
	state* openpar_intermediate = spec.create_state();
	spec.add_transition(start, "(", openpar_intermediate);
	state* openpar_state = spec.create_state(true, true, token_type::openpar);
	spec.add_else_transition(openpar_intermediate, openpar_state);
	// ) close paren token
	state* closepar_intermediate = spec.create_state();
	spec.add_transition(start, ")", closepar_intermediate);
	state* closepar_state = spec.create_state(true, true, token_type::closepar);
	spec.add_else_transition(closepar_intermediate, closepar_state);
	// { open brace token
	state* openbrace_intermediate = spec.create_state();
	spec.add_transition(start, "{", openbrace_intermediate);
	state* openbrace_state = spec.create_state(true, true, token_type::openbrace);
	spec.add_else_transition(openbrace_intermediate, openbrace_state);
	// } close brace token
	state* closebrace_intermediate = spec.create_state();
	spec.add_transition(start, "}", closebrace_intermediate);
	state* closebrace_state = spec.create_state(true, true, token_type::closebrace);
	spec.add_else_transition(closebrace_intermediate, closebrace_state);
	// [ open square token
	state* opensquare_intermediate = spec.create_state();
	spec.add_transition(start, "[", opensquare_intermediate);
	state* opensquare_state = spec.create_state(true, true, token_type::opensquare);
	spec.add_else_transition(opensquare_intermediate, opensquare_state);
	// ] close square token
	state* closesquare_intermediate = spec.create_state();
	spec.add_transition(start, "]", closesquare_intermediate);
	state* closesquare_state = spec.create_state(true, true, token_type::closesquare);
	spec.add_else_transition(closesquare_intermediate, closesquare_state);

	// ; semi colon token
	state* semicolon_intermediate = spec.create_state();
	spec.add_transition(start, ";", semicolon_intermediate);
	state* semicolon_state = spec.create_state(true, true, token_type::semicolon);
	spec.add_else_transition(semicolon_intermediate, semicolon_state);
	
}

specification::~specification() {
}

void specification::update_token_for_lexeme(token* t) {

	// To avoid adding more complexity to our dfa, we will change tokens depending on their lexeme
	if (t->type == token_type::id) {
		// Check to see if this identifier is in our reserved words set
		std::unordered_set<std::string>::const_iterator in_reserved_words = RESERVED_WORDS.find(t->lexeme);
		// If it is a reserved word, we will change the token type to a reserved word
		if (in_reserved_words != RESERVED_WORDS.end()) {
			t->type = token_type::reserved_word;
		}
		// Check to see if this identifier is in our token map
		std::unordered_map<std::string, token_type>::const_iterator in_token_map = TOKEN_MAP.find(t->lexeme);
		// If the lexeme is in our token map, then it is a identifer such as "and, or, not"
		// We will change it from an identifier token to it's respective and/or/not token
		if (in_token_map != TOKEN_MAP.end()) {
			t->type = in_token_map->second;
		}
	}
}

dfa specification::get_spec() {
	return spec;
}


void specification::whitespace_transitions(state* start, state* end) {
	spec.add_transition(start, " ", start);
	spec.add_transition(start, "\t", start);
	spec.add_transition(start, "\n", start);
	spec.add_transition(start, "\v", start);
	spec.add_transition(start, "\r", start);
	spec.add_transition(start, "\f", start);
	spec.add_transition(start, "\r\n", start);
}

void specification::nonzero_transitions(state* start, state* end) {
	std::string rule = "123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions 1 - 9 from the start state and end state
		// this will create 9 transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void specification::digit_transitions(state* start, state* end) {
	std::string rule = "0123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions 0 - 9 from the start state and end state
		// this will create 10 transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void specification::letter_transitions(state* start, state* end) {
	std::string rule = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions a - z and A - Z from the start state and end state
		// this will create 26 * 2 transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void specification::float_transitions(state* start, state* end) {
	state* intermediate1 = spec.create_state();
	state* intermediate2 = spec.create_state();
	integer_transitions(start, intermediate1, intermediate2);
	fraction_transitions(intermediate1, end);
}

void specification::fraction_transitions(state* start, state* end) {
	state* intermediate1 = spec.create_state();
	spec.add_transition(start, ".", intermediate1);
	digit_transitions(intermediate1, end);
	// if all the end transitions have already been created. we dont want to recreate them
	// we will just link the new intermediate to the same end
	// This can happen when there are two fraction tranistions coming out of two states
	// this is to avoid duplicate intermediate states
	if (spec.table(end->state_identifier, "0") == NULL) {
		nonzero_transitions(end, end);
		state* intermediate2 = spec.create_state();
		spec.add_transition(end, "0", intermediate2);
		spec.add_transition(intermediate2, "0", intermediate2);
		nonzero_transitions(intermediate2, end);
	}
}

void specification::integer_transitions(state* start, state* end, state* end2) {
	spec.add_transition(start, "0", end2);
	nonzero_transitions(start, end);
	digit_transitions(end, end);
}

void specification::num_transitions(state* start, state* end, state* end2, state* end3) {
	nonzero_transitions(start, end);
	digit_transitions(end, end);
	spec.add_transition(start, "0", end2);
	// Question: will this create duplicate inner states to be created
	// Answer: No. this is handled internally.
	fraction_transitions(end, end3);
	fraction_transitions(end2, end3);
}

void specification::alphanum_transitions(state* start, state* end) {
	spec.add_transition(start, "_", end);
	letter_transitions(start, end);
	digit_transitions(start, end);
}

void specification::id_transitions(state* start, state* end) {
	letter_transitions(start, end);
	alphanum_transitions(end, end);
}




