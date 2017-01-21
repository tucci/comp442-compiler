#include "stdafx.h"
#include "dfa_helper.h"

// TODO: test all of these

void dfa_helper::nonzero_transitions(dfa* spec, state* start, state* end) {
	std::string rule = "123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec->add_transition(start, rule.substr(i, 1), end);
	}
}

void dfa_helper::digit_transitions(dfa* spec, state* start, state* end) {
	std::string rule = "0123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec->add_transition(start, rule.substr(i, 1), end);
	}
}

void dfa_helper::letter_transitions(dfa* spec, state* start, state* end) {
	std::string rule = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec->add_transition(start, rule.substr(i, 1), end);
	}
}

void dfa_helper::float_transitions(dfa* spec, state* start, state* end) {
	state* intermediate1 = spec->create_state();
	state* intermediate2 = spec->create_state();
	integer_transitions(spec, start, intermediate1, intermediate2);
	fraction_transitions(spec, intermediate1, end);
}

void dfa_helper::fraction_transitions(dfa* spec, state* start, state* end) {	
	state* intermediate1 = spec->create_state();
	spec->add_transition(start, ".", intermediate1);
	digit_transitions(spec, intermediate1, end);
	// if all the end transitions have already been created. we dont want to recreate them
	// we will just link the new intermediate to the same end
	// This can happen when there are two fraction tranistions coming out of two states
	// this is to avoid duplicate intermediate states
	if (spec->table(end->state_identifier, "0") == NULL) {
		nonzero_transitions(spec, end, end);
		state* intermediate2 = spec->create_state();
		spec->add_transition(end, "0", intermediate2);
		spec->add_transition(intermediate2, "0", intermediate2);
		nonzero_transitions(spec, intermediate2, end);
	}
}

void dfa_helper::integer_transitions(dfa* spec, state* start, state* end, state* end2) {
	spec->add_transition(start, "0",  end2);
	nonzero_transitions(spec, start, end);
	digit_transitions(spec, end, end);
}

void dfa_helper::num_transitions(dfa* spec, state* start, state* end, state* end2, state* end3) {
	nonzero_transitions(spec, start, end);
	digit_transitions(spec, end, end);
	spec->add_transition(start, "0", end2);
	// Question: will this create duplicate inner states to be created
	// Answer: No. this is handled internally.
	fraction_transitions(spec, end, end3);
	fraction_transitions(spec, end2, end3);
}

void dfa_helper::alphanum_transitions(dfa* spec, state* start, state* end) {
	spec->add_transition(start, "_", end);
	letter_transitions(spec, start, end);
	digit_transitions(spec, start, end);
}

void dfa_helper::id_transitions(dfa* spec, state* start, state* end) {
	letter_transitions(spec, start, end);
	alphanum_transitions(spec, end, end);
}



