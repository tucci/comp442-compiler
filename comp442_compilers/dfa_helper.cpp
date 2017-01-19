#include "stdafx.h"
#include "dfa_helper.h"


void dfa_helper::nonzero_transitions(dfa spec, state start, state end) {
	std::string rule = "123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void dfa_helper::digit_transitions(dfa spec, state start, state end) {
	std::string rule = "0123456789";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void dfa_helper::letter_transitions(dfa spec, state start, state end) {
	std::string rule = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < rule.size(); i++) {
		// create transitions
		spec.add_transition(start, rule.substr(i, 1), end);
	}
}

void dfa_helper::float_transitions(dfa spec, state start, state end) {
	state intermediate1 = spec.create_state();
	integer_transitions(spec, start, intermediate1);
	state intermediate2 = spec.create_state();
	fraction_transitions(spec, intermediate1, intermediate2);
	spec.add_else_transition(intermediate2, end);
}


// TODO: figure this out
void dfa_helper::fraction_transitions(dfa spec, state start, state end) {

}

void dfa_helper::integer_transitions(dfa spec, state start, state end) {
	state intermediate1 = spec.create_state();
	nonzero_transitions(spec, start, intermediate1);
	digit_transitions(spec, intermediate1, intermediate1);
	spec.add_else_transition(intermediate1, end);
	state intermediate2 = spec.create_state();
	spec.add_transition(start, "0", intermediate2);
	spec.add_else_transition(intermediate2, end);
}

// TODO: figure this out. both start with a number
void dfa_helper::num_transitions(dfa spec, state start, state end) {

}

void dfa_helper::alphanum_transitions(dfa spec, state start, state end) {
	spec.add_transition(start, "_", end);
	letter_transitions(spec, start, end);
	digit_transitions(spec, start, end);
}

void dfa_helper::id_transitions(dfa spec, state start, state end) {
	state intermediate1 = spec.create_state();
	letter_transitions(spec, start, intermediate1);
	state intermediate2 = spec.create_state();
	alphanum_transitions(spec, intermediate1, intermediate2);
	alphanum_transitions(spec, intermediate2, intermediate2);
	spec.add_else_transition(intermediate2, end);
}



