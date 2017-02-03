#ifndef DFA_H
#define DFA_H



struct State {
	// integer value to hold the identifier for this state (ie. state 1, state 2, ...)
	int stateIdentifier;
	// Whether this state is a start state
	bool isStartState;
	// Whether this state is a final state
	bool isFinalState;
	// Whether this state needs to tell the lexer to backtrack
	// This will usually be true if there is an else transition to this state
	// Example the else state between < intermediate and < final state use an else transition between them
	// So the < final state needs to be a back tracking state
	// However you may not want this for the error state.
	// You must explicitly set this state when you create a state
	bool needsToBacktrack;
	// the token type for the lexeme it accepts
	TokenType tokenType;
	// Used for when this state is an error state
	ErrorType errorType;
};

class Dfa {
public:
	Dfa();
	~Dfa();
	// constant identifier to be used in the state transition table to use when no onther lookup is there
	const static std::string ELSE_TRANSITION;
	// Create the starting state for this dfa
	State* createStartState();
	// Create a state to be used in this dfa
	State* createState(bool isFinalState=false, bool needsToBacktrack=false, TokenType type=non_token);
	// add a transition from the fromState to the toState using a transition string
	bool addTransition(State* fromState, std::string transition, State* toState);
	// Add an else transition to the two states
	bool addElseTransition(State* fromState, State* toState);
	// Get the transition state from the fromState identifier using lookupTransition input
	State* table(int fromState, std::string lookupTransition);
	// Check to see if the dfa has the current state
	bool hasState(State state);
	// Returns the state that this input leads to
	State* stateFromInput(std::string input);
	// Check to see if the given input is accepted in our dfa
	bool acceptsInput(std::string);
	// Returns the starting state for this dfa
	State* getStartingState();
private:
	// the count for how many states we have
	int mStateCount;
	// the data structure that stores the state transition table
	// where the key to this map is the state identifier
	// the value to for the specified key, is the transition map for that state
	std::unordered_map<int, std::shared_ptr<std::unordered_map<std::string, State*>>> stateTransitionTable;
	std::vector<State*> states;
	State* startingState;
};

#endif

