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

	TEST_METHOD(Symbol_NonTerminalSepcialSymbolTest) {
	};

	TEST_METHOD(Symbol_TerminalTest) {
		Terminal t("<test>");
		Assert::IsTrue(t.getName() == "<test>");
		Assert::IsTrue(t.isTerminal());
	};

	};
}