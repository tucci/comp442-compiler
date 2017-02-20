#ifndef SPECIFICATION_H
#define SPECIFICATION_H


// This class is specifically used to set the language specs
// We set the specification by setting the specs directly into the dfa
class Specification {
public:
	// Holds all the mappings from lexemes to token types for our language. Does not include ids or number tokens
	const static std::unordered_map<std::string, TokenType> TOKEN_MAP;
	// A map just to hold the strings of the tokens used when printing/outputing to file.
	// So that when reading the token types, u dont have to look up the int value
	// Besides pretty printing, this is not used anywhere else
	const static std::unordered_map<TokenType, std::string> TOKEN_PRINT_MAP;

	// If use default, it will use the spec provided. if false, the spec will be empty
	Specification(bool useDefault = true);
	~Specification();
	// Returns the dfa for this specification
	std::shared_ptr<Dfa> getSpec();
	// Returns the token type for the given input
	// Used in testing
	TokenType getTokenTypeForInput(std::string input);
	// Updates the token type depending on the lexeme
	static void updateTokenType(Token* token);
	// Create white space transitions from the from the start state to the end state
	void whitespaceTransitions(State* start, State* end);
	// Create non zero transitions regex="[0-9]" from the start state to the end state
	void nonzeroTransitions(State* start, State* end);
	// Create non digit transitions regex="[1-9]" from the start state to the end state
	void digitTransitions(State* start, State* end);
	// Create letter transitions regex="[a-z|A-Z]" from the start state to the end state
	void letterTransitions(State* start, State* end);
	// Create fraction transitions regex=".0|.digit*" from the start state to the end state
	void fractionTransitions(State* start, State* end);
	// Create float transitions regex="integer fraction" from the start state to the end state
	void floatTransitions(State* start, State* end);
	// Create integer transitions regex="integer fraction" from the start state to the end state and end state 2
	// The integer transition requires 2 ends states. So you must supply 2 end states
	// If you want to create a bigger machine with this transition
	// make sure to connect both end states to the new starting state of the new machine
	void integerTransitions(State* start, State* end, State* end2);
	//Create num transitions regex="integer|float" from the start state to the end state
	// The num transition requires 3 ends states. So you must supply 3 end states
	// If you want to create a bigger machine with this transition
	// make sure to connect all end states to the new starting state of the new machine
	void numTransitions(State* start, State* end, State* end2, State* end3);
	//Create alphanum transitions regex="letter|digit|_*" from the start state to the end state
	void alphanumTransitions(State* start, State* end);
	//Create id transitions regex="letter alphanum*" from the start state to the end state
	void idTransitions(State* start, State* end);
private:
	// The dfa/state transition table for this spec
	std::shared_ptr<Dfa> spec;
};

#endif // !SPECIFICATION_H