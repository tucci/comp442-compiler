#ifndef DFA_HELPER_H
#define DFA_HELPER_H

// This class is used to help generate dfa transitions easily from our grammar rule
class dfa_helper {
public:
	static void nonzero_transitions(dfa, state, state);
	static void digit_transitions(dfa, state, state);
	static void letter_transitions(dfa, state, state);
	static void fraction_transitions(dfa, state, state);
	static void float_transitions(dfa, state, state);
	static void integer_transitions(dfa, state, state);
	static void num_transitions(dfa, state, state);
	static void alphanum_transitions(dfa, state, state);
	static void id_transitions(dfa, state, state);

};

#endif // !DFA_HELPER_H