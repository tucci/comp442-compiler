#include "stdafx.h"

const std::string Dfa::ELSE_TRANSITION = "ELSE_TRANSITION";

Dfa::Dfa() {
	mStateCount = 0;
}

Dfa::~Dfa() {
	// Delete states
	for (std::vector<State*>::iterator it = states.begin(); it != states.end(); ++it) {
		delete *it;
		*it = NULL;
	}
		
}

State* Dfa::createStartState() {
	// create as it were a normal state
	State* startState = createState(false);
	// set this state to a start state
	startState->isStartState = true;
	return startState;
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

