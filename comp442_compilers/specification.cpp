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

specification::specification() {
	// Here we hardcode the spec into the dfa
	state* start = spec.create_start_state();
	// TODO: also need to include other whitespace characters here
	spec.add_transition(start, " ", start);

	// id state transitions
	state* i1 = spec.create_state();
	state* id_state = spec.create_state(true, false, token_type::id);
	specification::id_transitions(start, i1);
	spec.add_else_transition(i1, id_state);

	// num state transitions
	state* i2 = spec.create_state();
	state* i3 = spec.create_state();
	state* i4 = spec.create_state();
	state* num_state = spec.create_state(true, false, token_type::num);
	specification::num_transitions(start, i2, i3, i4);
	spec.add_else_transition(i2, num_state);
	spec.add_else_transition(i3, num_state);
	spec.add_else_transition(i4, num_state);
}

specification::~specification() {
}

bool specification::is_reserved_word(std::string lexeme) {
	std::unordered_set<std::string>::iterator in_set = RESERVED_WORDS.find(lexeme);
	return in_set != RESERVED_WORDS.end();
}

dfa specification::get_spec() {
	return spec;
}



void specification::nonzero_transitions(state* start, state* end) {
	std::string rule = "123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void specification::digit_transitions(state* start, state* end) {
	std::string rule = "0123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void specification::letter_transitions(state* start, state* end) {
	std::string rule = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
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




