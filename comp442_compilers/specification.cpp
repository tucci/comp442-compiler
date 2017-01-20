#include "stdafx.h"
#include "specification.h"


specification::specification() {
	// Here we hardcode the spec into the dfa
	state* start = spec.create_start_state();
	// TODO: also need to include other whitespace characters here
	spec.add_transition(start, " ", start);

	// id state transitions
	state* i1 = spec.create_state();
	state* id_state = spec.create_state(true, false, token_type::id);
	dfa_helper::id_transitions(&spec, start, i1);
	spec.add_else_transition(i1, id_state);

	// num state transitions
	state* i2 = spec.create_state();
	state* i3 = spec.create_state();
	state* i4 = spec.create_state();
	state* num_state = spec.create_state(true, false, token_type::num);
	dfa_helper::num_transitions(&spec, start, i2, i3, i4);
	spec.add_else_transition(i2, num_state);
	spec.add_else_transition(i3, num_state);
	spec.add_else_transition(i4, num_state);
}


specification::~specification() {
}

dfa specification::get_spec() {
	return spec;
}
