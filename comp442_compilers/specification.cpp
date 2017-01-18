#include "stdafx.h"
#include "specification.h"


specification::specification() {
	// Here we hardcode the spec into the dfa
	state start = spec.create_start_state();
	state s2 = spec.create_state(true, non_zero_t);
	spec.add_transition(start, " ", start);
	spec.add_transition(start, "1", s2);
	spec.add_transition(start, "2", s2);
	spec.add_transition(start, "3", s2);
	spec.add_transition(start, "4", s2);
	spec.add_transition(start, "5", s2);
	spec.add_transition(start, "6", s2);
	spec.add_transition(start, "7", s2);
	spec.add_transition(start, "8", s2);
	spec.add_transition(start, "9", s2);
}


specification::~specification() {
}

dfa specification::get_spec() {
	return spec;
}
