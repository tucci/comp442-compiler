#ifndef SPECIFICATION_H
#define SPECIFICATION_H


// This class is specifically used to set the language specs
// We set the specification by settings the specs directly into the dfa
class specification {
public:
	specification();
	~specification();
	// This will be used to turn identifiers to reserved words and word operators such as "and, or, not" into their tokens
	static void update_token_for_lexeme(token*);
	// Returns the dfa for this specification
	dfa get_spec();
private:
	// Holds all the keywords for our language
	const static std::unordered_set<std::string> RESERVED_WORDS;
	// Holds all the mappings from lexems to token types for our language. Does not include ids/reserved words/ or number tokens
	const static std::unordered_map<std::string, token_type> TOKEN_MAP;

	// The dfa/state transition table for this spec
	dfa spec;
	// Create non zero transitions regex="[0-9]" from the start state to the end state
	void nonzero_transitions(state* start, state* end);
	// Create non digit transitions regex="[1-9]" from the start state to the end state
	void digit_transitions(state* start, state* end);
	// Create letter transitions regex="[a-z|A-Z]" from the start state to the end state
	void letter_transitions(state* start, state* end);
	// Create fraction transitions regex=".0|.digit*" from the start state to the end state
	void fraction_transitions(state* start, state* end);
	// Create float transitions regex="integer fraction" from the start state to the end state
	void float_transitions(state* start, state* end);
	// Create integer transitions regex="integer fraction" from the start state to the end state and end state 2
	// The integer transition requires 2 ends states. So you must supply 2 end states
	// If you want to create a bigger machine with this transition
	// make sure to connect both end states to the new starting state of the new machine
	void integer_transitions(state* start, state* end, state* end2);
	//Create num transitions regex="integer|float" from the start state to the end state
	// The num transition requires 3 ends states. So you must supply 3 end states
	// If you want to create a bigger machine with this transition
	// make sure to connect all end states to the new starting state of the new machine
	void num_transitions(state* start, state* end, state* end2, state* end3);
	//Create alphanum transitions regex="letter|digit|_*" from the start state to the end state
	void alphanum_transitions(state* start, state* end);
	//Create id transitions regex="letter alphanum*" from the start state to the end state
	void id_transitions(state* start, state* end);
};

#endif // !SPECIFICATION_H