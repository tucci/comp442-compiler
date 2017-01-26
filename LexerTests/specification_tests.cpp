#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LexerTests {
	TEST_CLASS(SpecificationTest) {
public:

	TEST_METHOD(whitespaceTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* expected = tokenizer->createState();
		spec.whitespaceTransitions(start, expected);
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, " "));
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, "\t"));
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, "\n"));
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, "\v"));
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, "\r"));
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, "\f"));
		Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, "\r\n"));
	};

	TEST_METHOD(nonZeroTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* expected = tokenizer->createState();
		spec.nonzeroTransitions(start, expected);
		// The zero should return NULL
		Assert::IsFalse(expected == tokenizer->table(start->stateIdentifier, "0"));

		std::string rule = "123456789";

		for (int i = 0; i < rule.size(); i++) {
			// Get the digit at i th position
			std::string c = rule.substr(i, 1);
			// Assert that the state is there
			Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, c));
		}
	};

	TEST_METHOD(digitTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* expected = tokenizer->createState();
		spec.digitTransitions(start, expected);
		// The zero should return a state
		std::string rule = "0123456789";
		for (int i = 0; i < rule.size(); i++) {
			// Get the digit at i th position
			std::string c = rule.substr(i, 1);
			// Assert that the state is there
			Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, c));
		}
	};

	TEST_METHOD(letterTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* expected = tokenizer->createState();
		spec.letterTransitions(start, expected);
		// The zero should return a state
		std::string rule = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		for (int i = 0; i < rule.size(); i++) {
			// Get the digit at i th position
			std::string c = rule.substr(i, 1);
			// Assert that the state is there
			Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, c));
		}
	};

	TEST_METHOD(floatTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* end = tokenizer->createState(true);
		spec.floatTransitions(start, end);

		// Should not end with zero
		Assert::IsFalse(tokenizer->acceptsInput("1.30"));
		// Should not start with zero
		Assert::IsFalse(tokenizer->acceptsInput("01.30"));
		// Should accept x.0
		Assert::IsTrue(tokenizer->acceptsInput("1.0"));
		// Should not accept 0.0
		Assert::IsFalse(tokenizer->acceptsInput("0.0"));
		// Should not accept 0.x, The spec doesnt allow for this. but it is considered a fraction
		Assert::IsFalse(tokenizer->acceptsInput("0.3"));
		Assert::IsTrue(tokenizer->acceptsInput("1.1"));
		Assert::IsTrue(tokenizer->acceptsInput("1.9"));
		Assert::IsTrue(tokenizer->acceptsInput("4.0009"));
		Assert::IsTrue(tokenizer->acceptsInput("4.090032"));
		Assert::IsTrue(tokenizer->acceptsInput("40303030.090032"));
		Assert::IsTrue(tokenizer->acceptsInput("434343.12323232"));
		Assert::IsTrue(tokenizer->acceptsInput("12.3"));

		Assert::IsFalse(tokenizer->acceptsInput("0434343.12323232"));
		Assert::IsFalse(tokenizer->acceptsInput("232"));
		Assert::IsFalse(tokenizer->acceptsInput("0"));
		Assert::IsFalse(tokenizer->acceptsInput("000"));
		Assert::IsFalse(tokenizer->acceptsInput("000.000"));
	};


	TEST_METHOD(fractionTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* end = tokenizer->createState(true);
		spec.fractionTransitions(start, end);

		// Should accept .0
		Assert::IsTrue(tokenizer->acceptsInput(".0"));
		Assert::IsTrue(tokenizer->acceptsInput(".1"));
		Assert::IsTrue(tokenizer->acceptsInput(".5"));
		Assert::IsTrue(tokenizer->acceptsInput(".9"));
		Assert::IsTrue(tokenizer->acceptsInput(".0000000009"));
		Assert::IsTrue(tokenizer->acceptsInput(".99999"));
		Assert::IsTrue(tokenizer->acceptsInput(".9000003232"));

		Assert::IsFalse(tokenizer->acceptsInput(".00"));
		Assert::IsFalse(tokenizer->acceptsInput(".44444440"));
		Assert::IsFalse(tokenizer->acceptsInput(".10"));
		Assert::IsFalse(tokenizer->acceptsInput(".90"));

		Assert::IsFalse(tokenizer->acceptsInput("2"));
		Assert::IsFalse(tokenizer->acceptsInput("2.33"));
	};


	TEST_METHOD(integerTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* end = tokenizer->createState(true);
		State* end2 = tokenizer->createState(true);
		spec.integerTransitions(start, end, end2);

		// Should accept 0
		Assert::IsTrue(tokenizer->acceptsInput("0"));
		Assert::IsTrue(tokenizer->acceptsInput("1"));
		Assert::IsTrue(tokenizer->acceptsInput("2"));
		Assert::IsTrue(tokenizer->acceptsInput("3"));
		Assert::IsTrue(tokenizer->acceptsInput("4"));
		Assert::IsTrue(tokenizer->acceptsInput("5"));
		Assert::IsTrue(tokenizer->acceptsInput("6"));
		Assert::IsTrue(tokenizer->acceptsInput("7"));
		Assert::IsTrue(tokenizer->acceptsInput("8"));
		Assert::IsTrue(tokenizer->acceptsInput("9"));
		Assert::IsTrue(tokenizer->acceptsInput("10"));
		Assert::IsTrue(tokenizer->acceptsInput("24550"));
		Assert::IsTrue(tokenizer->acceptsInput("3055850"));

		Assert::IsFalse(tokenizer->acceptsInput("00"));
		Assert::IsFalse(tokenizer->acceptsInput("000042"));
		Assert::IsFalse(tokenizer->acceptsInput("02320"));

		Assert::IsFalse(tokenizer->acceptsInput("232.2"));
		Assert::IsFalse(tokenizer->acceptsInput("1.0"));
		Assert::IsFalse(tokenizer->acceptsInput("01.0"));

	};


	TEST_METHOD(numTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* end = tokenizer->createState(true);
		State* end2 = tokenizer->createState(true);
		State* end3 = tokenizer->createState(true);
		spec.numTransitions(start, end, end2, end3);

		// Floats
		// Should not end with zero
		Assert::IsFalse(tokenizer->acceptsInput("1.30"));
		// Should not start with zero
		Assert::IsFalse(tokenizer->acceptsInput("01.30"));
		// Should accept x.0
		Assert::IsTrue(tokenizer->acceptsInput("1.0"));

		// Should not accept 0.0
		Assert::IsTrue(tokenizer->acceptsInput("0.0"));

		// Should not accept 0.x, The spec doesnt allow for this. but it is considered a fraction
		Assert::IsTrue(tokenizer->acceptsInput("0.3"));
		Assert::IsTrue(tokenizer->acceptsInput("1.1"));
		Assert::IsTrue(tokenizer->acceptsInput("1.9"));
		Assert::IsTrue(tokenizer->acceptsInput("4.0009"));
		Assert::IsTrue(tokenizer->acceptsInput("4.090032"));
		Assert::IsTrue(tokenizer->acceptsInput("40303030.090032"));
		Assert::IsTrue(tokenizer->acceptsInput("434343.12323232"));
		Assert::IsTrue(tokenizer->acceptsInput("12.3"));

		Assert::IsFalse(tokenizer->acceptsInput("0434343.12323232"));
		Assert::IsTrue(tokenizer->acceptsInput("0"));
		Assert::IsFalse(tokenizer->acceptsInput("000"));
		Assert::IsFalse(tokenizer->acceptsInput("000.000"));

		// Integers
		// Should accept 0
		Assert::IsTrue(tokenizer->acceptsInput("0"));
		Assert::IsTrue(tokenizer->acceptsInput("1"));
		Assert::IsTrue(tokenizer->acceptsInput("2"));
		Assert::IsTrue(tokenizer->acceptsInput("3"));
		Assert::IsTrue(tokenizer->acceptsInput("4"));
		Assert::IsTrue(tokenizer->acceptsInput("5"));
		Assert::IsTrue(tokenizer->acceptsInput("6"));
		Assert::IsTrue(tokenizer->acceptsInput("7"));
		Assert::IsTrue(tokenizer->acceptsInput("8"));
		Assert::IsTrue(tokenizer->acceptsInput("9"));
		Assert::IsTrue(tokenizer->acceptsInput("10"));
		Assert::IsTrue(tokenizer->acceptsInput("24550"));
		Assert::IsTrue(tokenizer->acceptsInput("3055850"));

		Assert::IsFalse(tokenizer->acceptsInput("00"));
		Assert::IsFalse(tokenizer->acceptsInput("000042"));
		Assert::IsFalse(tokenizer->acceptsInput("02320"));

		Assert::IsTrue(tokenizer->acceptsInput("232.2"));
		Assert::IsFalse(tokenizer->acceptsInput("01.0"));

	};



	TEST_METHOD(alphaNumTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* expected = tokenizer->createState(true);
		spec.alphanumTransitions(start, expected);
		std::string rule = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		for (int i = 0; i < rule.size(); i++) {
			// Get the digit at i th position
			std::string c = rule.substr(i, 1);
			// Assert that the state is there
			Assert::IsTrue(expected == tokenizer->table(start->stateIdentifier, c));
		}
	};


	TEST_METHOD(idTokenTest) {
		Specification spec(false);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();
		State* start = tokenizer->createStartState();
		State* expected = tokenizer->createState(true);
		spec.idTransitions(start, expected);
		Assert::IsTrue(tokenizer->acceptsInput("a"));
		Assert::IsTrue(tokenizer->acceptsInput("z"));
		Assert::IsTrue(tokenizer->acceptsInput("methodName"));
		Assert::IsTrue(tokenizer->acceptsInput("methodName22"));
		Assert::IsTrue(tokenizer->acceptsInput("method_name"));
		Assert::IsTrue(tokenizer->acceptsInput("method_name22"));
		Assert::IsTrue(tokenizer->acceptsInput("method_name_22"));
		Assert::IsTrue(tokenizer->acceptsInput("method_name__22"));
		Assert::IsTrue(tokenizer->acceptsInput("method_name__22_"));
		Assert::IsTrue(tokenizer->acceptsInput("ClassName"));
		Assert::IsTrue(tokenizer->acceptsInput("Class_Name"));
		Assert::IsTrue(tokenizer->acceptsInput("ALLCAPS")); // :)
		Assert::IsTrue(tokenizer->acceptsInput("lowercase"));
		Assert::IsTrue(tokenizer->acceptsInput("a_____________"));

		Assert::IsFalse(tokenizer->acceptsInput("_"));
		Assert::IsFalse(tokenizer->acceptsInput("_a"));
		Assert::IsFalse(tokenizer->acceptsInput("_A"));
		Assert::IsFalse(tokenizer->acceptsInput("___"));

	};


	TEST_METHOD(specificationTokenTest) {
		Specification spec(true);
		std::shared_ptr<Dfa> tokenizer = spec.getSpec();

		// Test all token types
		std::unordered_map<std::string, TokenType>::const_iterator it = Specification::TOKEN_MAP.begin();
		while (it != Specification::TOKEN_MAP.end()) {
			TokenType type = spec.getTokenTypeForInput(it->first);
			Assert::IsTrue(it->second == type);
			it++;
		}

	};


	};
}