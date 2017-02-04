#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ProductionTest) {
public:

	TEST_METHOD(Production_creationTest) {
		Terminal x("x");
		NonTerminal a("A");
		NonTerminal b("B");
		NonTerminal S("S");
		std::vector<Symbol> prod = { x,a,b };
		Production p(S, prod);
		Assert::IsTrue(S.getName() == p.getNonTerminal().getName());
		Assert::IsTrue(prod.at(0).getName() == x.getName());
		Assert::IsTrue(prod.at(1).getName() == a.getName());
		Assert::IsTrue(prod.at(2).getName() == b.getName());
	};

	TEST_METHOD(Production_creationNoNonTerminalTest) {
		
	};


	};
}