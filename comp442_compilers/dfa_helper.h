#ifndef DFA_HELPER_H
#define DFA_HELPER_H

// This class is used to help generate dfa transitions easily from our grammar rule
class dfa_helper {
	// TODO: write tests
public:
	// Create non zero transitions regex="[0-9]" from the start state to the end state
	static void nonzero_transitions(dfa* spec, state* start, state* end);
	// Create non digit transitions regex="[1-9]" from the start state to the end state
	static void digit_transitions(dfa* spec, state* start, state* end);
	// Create letter transitions regex="[a-z|A-Z]" from the start state to the end state
	static void letter_transitions(dfa* spec, state* start, state* end);
	// Create fraction transitions regex=".0|.digit*" from the start state to the end state
	static void fraction_transitions(dfa* spec, state* start, state* end);
	// Create float transitions regex="integer fraction" from the start state to the end state
	static void float_transitions(dfa* spec, state* start, state* end);
	// Create integer transitions regex="integer fraction" from the start state to the end state and end state 2
	// The integer transition requires 2 ends states. So you must supply 2 end states
	// If you want to create a bigger machine with this transition
	// make sure to connect both end states to the new starting state of the new machine
	static void integer_transitions(dfa* spec, state* start, state* end, state* end2);
	//Create num transitions regex="integer|float" from the start state to the end state
	// The num transition requires 3 ends states. So you must supply 3 end states
	// If you want to create a bigger machine with this transition
	// make sure to connect all end states to the new starting state of the new machine
	static void num_transitions(dfa* spec, state* start, state* end, state* end2, state* end3);
	//Create alphanum transitions regex="letter|digit|_*" from the start state to the end state
	static void alphanum_transitions(dfa* spec, state* start, state* end);
	//Create id transitions regex="letter alphanum*" from the start state to the end state
	static void id_transitions(dfa* spec, state* start, state* end);

};

#endif // !DFA_HELPER_H