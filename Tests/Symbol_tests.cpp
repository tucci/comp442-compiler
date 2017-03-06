#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(SymbolTest) {
public:

	TEST_METHOD(Symbol_NonTerminalTest) {
		NonTerminal nt("s");
		Assert::IsTrue(nt.getName() == "s");
		Assert::IsFalse(nt.isTerminal());
	};

	TEST_METHOD(Symbol_TerminalTest) {
		Terminal t("<test>");
		Assert::IsTrue(t.getName() == "<test>");
		Assert::IsTrue(t.isTerminal());
	};

	TEST_METHOD(Symbol_SepcialSymbolTest) {
		// Here we are just testing to see if the same are the same
		Assert::IsTrue("EPSILON" == SpecialTerminal::EPSILON.getName());
		Assert::IsTrue("$" == SpecialTerminal::END_OF_FILE.getName());

		Assert::IsTrue(SpecialTerminal::isEpsilon(SpecialTerminal::EPSILON.getName()));
		Assert::IsFalse(SpecialTerminal::isEpsilon(SpecialTerminal::END_OF_FILE.getName()));

		Terminal t("notEpsilon");
		NonTerminal nt("notEpsilon2");

		Assert::IsFalse(SpecialTerminal::isEpsilon(t.getName()));
		Assert::IsFalse(SpecialTerminal::isEpsilon(nt.getName()));

	};

	

	};
}