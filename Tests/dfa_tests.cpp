#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(DfaTest)
	{
	public:

		TEST_METHOD(getStartingStateTest) {
			Dfa dfa;
			State* test = dfa.createStartState();
			Assert::IsTrue(test == dfa.getStartingState());
		};

		TEST_METHOD(createStartState1) {
			Dfa dfa;
			State* test = dfa.createStartState();
			Assert::IsTrue(test->isStartState);
		};

		TEST_METHOD(createStartState2) {
			Dfa dfa;
			State* start1 = dfa.createStartState();
			State* test3 = dfa.createState();
			State* start2 = dfa.createStartState();
			Assert::IsFalse(start1->isStartState);
			Assert::IsTrue(start2->isStartState);
		};

		TEST_METHOD(createStateDefault) {
			Dfa dfa;
			State* test = dfa.createState();
			Assert::IsFalse(test->isStartState);
			Assert::IsFalse(test->isFinalState);
			Assert::AreEqual(static_cast<int>(TokenType::non_token), static_cast<int>(test->tokenType));
		}

		TEST_METHOD(createStateFinal) {
			Dfa dfa;
			State* test = dfa.createState(true);
			Assert::IsTrue(test->isFinalState);
		}

		TEST_METHOD(createStateBacktrack) {
			Dfa dfa;
			State* test = dfa.createState(false, true);
			Assert::IsTrue(test->needsToBacktrack);
		}

		TEST_METHOD(createStateWithTokenType) {
			Dfa dfa;
			State* test = dfa.createState(false, false, TokenType::id);
			Assert::AreEqual(static_cast<int>(TokenType::id), static_cast<int>(test->tokenType));
		}

		TEST_METHOD(addTransitionSuccessTrue) {
			Dfa dfa;
			std::string transition = "a";
			State* start = dfa.createStartState();
			State* end = dfa.createState();
			// Testing to see if the transition was added successfully
			bool actual = dfa.addTransition(start, transition, end);
			Assert::IsTrue(actual);
		}

		TEST_METHOD(addTransitionSuccessFalse) {
			Dfa dfa;
			std::string transition = "a";
			State* start = dfa.createStartState();
			State* end = new State();
			// Testing to see if the transition was added successfully
			bool actual = dfa.addTransition(start, transition, end);
			Assert::IsFalse(actual);
			delete end;
		}

		TEST_METHOD(addTransitionSuccessFalse2) {
			Dfa dfa;
			std::string transition = "a";
			State* start = new State();
			State* end = new State();
			// Testing to see if the transition was added successfully
			bool actual = dfa.addTransition(start, transition, end);
			Assert::IsFalse(actual);
			delete start;
			delete end;
		}

		TEST_METHOD(addElseTransition) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState();
			// Testing to see if the else transition was added successfully
			bool actual = dfa.addElseTransition(start, end);
			Assert::IsTrue(actual);
		}

		TEST_METHOD(hasStateTrue) {
			Dfa dfa;
			State* start = dfa.createStartState();
			// Testing to see if the state we just added is in the dfa
			bool actual = dfa.hasState(*start);
			Assert::IsTrue(actual);
		}

		TEST_METHOD(hasStateTrue2) {
			Dfa dfa;
			State* state = dfa.createState();
			// Testing to see if the state we just added is in the dfa
			bool actual = dfa.hasState(*state);
			Assert::IsTrue(actual);
		}

		TEST_METHOD(hasStateFalse) {
			Dfa dfa;
			State state;
			state.stateIdentifier = 2;
			// Testing to see if the state we just added is in the dfa
			bool actual = dfa.hasState(state);
			Assert::IsFalse(actual);
		}

		TEST_METHOD(tableSuccess) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState();
			std::string transition = "a";
			dfa.addTransition(start, transition, end);
			State* actual = dfa.table(start->stateIdentifier, transition);
			// See if the end state we created is the same as the end state we got from the table
			Assert::AreEqual(end->stateIdentifier, actual->stateIdentifier);
		}


		TEST_METHOD(tableSuccessElseTransiton) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState();
			dfa.addElseTransition(start, end);
			State* actual = dfa.table(start->stateIdentifier, Dfa::ELSE_TRANSITION);
			// See if the end state we created is the same as the end state we got from the table
			Assert::AreEqual(end->stateIdentifier, actual->stateIdentifier);
		}

		TEST_METHOD(tableNULLTest) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* actual = dfa.table(start->stateIdentifier, "a");
			// See if we get a NULL from a transition that doesnt exist
			Assert::IsNull(actual);
		}

		TEST_METHOD(tableNULLTest2) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState();
			dfa.addTransition(start, "a", end);
			State* actual = dfa.table(start->stateIdentifier, "b");
			// See if we get a NULL from a transition that doesnt exist
			Assert::IsNull(actual);
		}

		TEST_METHOD(acceptsInputTest1) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* i1 = dfa.createState();
			State* i2 = dfa.createState();
			State* end = dfa.createState(true);
			dfa.addTransition(start, "a", i1);
			dfa.addTransition(i1, "b", i2);
			dfa.addTransition(i2, "c", end);
			// creates dfa that accepts abc as input
			Assert::IsTrue(dfa.acceptsInput("abc"));
			Assert::IsFalse(dfa.acceptsInput("a"));
			Assert::IsFalse(dfa.acceptsInput("ab"));
			Assert::IsFalse(dfa.acceptsInput("abcc"));
			Assert::IsFalse(dfa.acceptsInput("b"));
			Assert::IsFalse(dfa.acceptsInput("c"));
			Assert::IsFalse(dfa.acceptsInput("bac"));
		}
		
		TEST_METHOD(acceptsInputTest2) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState(true);
			dfa.addTransition(start, "a", start);
			dfa.addTransition(start, "b", end);
			// creates dfa that accepts a*b as input
			Assert::IsTrue(dfa.acceptsInput("b"));
			Assert::IsTrue(dfa.acceptsInput("ab"));
			Assert::IsTrue(dfa.acceptsInput("aab"));
			Assert::IsTrue(dfa.acceptsInput("aaab"));
			Assert::IsTrue(dfa.acceptsInput("aaaaaaaaaab"));
			Assert::IsFalse(dfa.acceptsInput("a"));
			Assert::IsFalse(dfa.acceptsInput("abb"));
			Assert::IsFalse(dfa.acceptsInput("abbb"));
		}

		TEST_METHOD(acceptsInputTest3) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState(true);
			dfa.addTransition(start, "a", start);
			dfa.addTransition(start, "b", end);
			dfa.addTransition(end, "b", end);
			// creates dfa that accepts a*bb* as input
			Assert::IsTrue(dfa.acceptsInput("b"));
			Assert::IsTrue(dfa.acceptsInput("ab"));
			Assert::IsTrue(dfa.acceptsInput("aab"));
			Assert::IsTrue(dfa.acceptsInput("aaab"));
			Assert::IsTrue(dfa.acceptsInput("aaaaaaaaaab"));
			Assert::IsTrue(dfa.acceptsInput("abb"));
			Assert::IsTrue(dfa.acceptsInput("bb"));
			Assert::IsTrue(dfa.acceptsInput("bbbbbbbbbbbb"));
			Assert::IsFalse(dfa.acceptsInput("a"));
			Assert::IsFalse(dfa.acceptsInput("aaa"));	
		}

		TEST_METHOD(stateFromInputTest1) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* i1 = dfa.createState();
			State* i2 = dfa.createState();
			State* end = dfa.createState(true);
			// abc
			dfa.addTransition(start, "a", i1);
			dfa.addTransition(i1, "b", i2);
			dfa.addTransition(i2, "c", end);
			Assert::IsTrue(end == dfa.stateFromInput("abc"));
			
		}

		TEST_METHOD(stateFromInputTest2) {
			Dfa dfa;
			State* start = dfa.createStartState();
			State* end = dfa.createState(true);
			dfa.addTransition(start, "a", start);
			dfa.addTransition(start, "b", end);
			dfa.addTransition(end, "b", end);
			// creates dfa that accepts a*bb* as input
			Assert::IsTrue(end == dfa.stateFromInput("aaaaaaaaaab"));
	
		}

	};
}