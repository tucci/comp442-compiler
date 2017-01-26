#include "stdafx.h"

const std::string Dfa::ELSE_TRANSITION = "ELSE_TRANSITION";

Dfa::Dfa() {
	mStateCount = 0;
	startingState = NULL;
}

Dfa::~Dfa() {
	// Delete states
	for (std::vector<State*>::iterator it = states.begin(); it != states.end(); ++it) {
		delete *it;
		*it = NULL;
	}
		
}

State* Dfa::createStartState() {
	if (startingState == NULL) {
		// This is the first time creating the start state
		startingState = createState(false);
	} else {
		// Set the old starting state to false
		startingState->isStartState = false;
		// We are creating another starting state
		startingState = createState(false);
	}	

	// set this state to a start state
	startingState->isStartState = true;
	return startingState;
}

State* Dfa::createState(bool isFinalState, bool needsToBacktrack, TokenType type) {
	// init state with the incremented state identifier and specified isFinalState
	State* newState = new State{ ++mStateCount, false, isFinalState, needsToBacktrack, type };
	states.push_back(newState);
	// create the transition map for this state
	std::shared_ptr<std::unordered_map<std::string, State*>> transitions = std::shared_ptr<std::unordered_map<std::string, State*>>(new std::unordered_map<std::string, State*>);
	// add the transition map to our state transition table
	stateTransitionTable.emplace(newState->stateIdentifier, transitions);
	return newState;
}

bool Dfa::addTransition(State* fromState, std::string transition, State* toState) {
	// We need to check if these states are in the table
	if (hasState(*fromState) && hasState(*toState)) {
		// Get this transitions for the from state
		std::shared_ptr<std::unordered_map<std::string, State*>> stateTransitions = stateTransitionTable.at(fromState->stateIdentifier);
		stateTransitions->emplace(transition, toState);
		return true;
	}
	return false;
}

bool Dfa::addElseTransition(State* fromState, State* toState) {
	return addTransition(fromState, ELSE_TRANSITION, toState);
}

bool Dfa::hasState(State state) {
	return stateTransitionTable.find(state.stateIdentifier) != stateTransitionTable.end();
}

State* Dfa::table(int fromState, std::string lookupTransition) {
	std::shared_ptr<std::unordered_map<std::string, State*>> stateTransitions = stateTransitionTable.at(fromState);
	std::unordered_map<std::string, State*>::iterator found = stateTransitions->find(lookupTransition);
	// If we dont have a transition for this state, return NULL, the caller must handle going to the else state
	if (found == stateTransitions->end()) {
		return NULL;
	}
	// if we did find something, then return that transition state
	return found->second;
}

State* Dfa::stateFromInput(std::string input) {
	bool isAccepted = false;
	int str_index = 0;
	std::string lookupTransition;
	State* currentState = startingState;


	while (!isAccepted && str_index <= input.length()) {
		lookupTransition = input.substr(str_index, 1);
		State* nextState = table(currentState->stateIdentifier, lookupTransition);

		if (nextState != NULL) {
			// Move to next state
			currentState = nextState;
		} else {
			// Check if we have an else state
			State* elseState = table(currentState->stateIdentifier, ELSE_TRANSITION);
			if (elseState == NULL) {
				// No else state. dfa doenst accept this input
				return NULL;
			}
			// Move to else state
			currentState = elseState;
		}

		if (currentState != NULL) {
			str_index++;
			if (currentState->isFinalState && str_index >= input.length()) {
				return currentState;
			}
		} else {
			return NULL;
		}
	}
	return NULL;
}

bool Dfa::acceptsInput(std::string testString) {
	return stateFromInput(testString) != NULL;
}

State* Dfa::getStartingState() {
	return startingState;
}

