#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ProductionTest) {
public:

	TEST_METHOD(Production_creationNormalTest) {
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

	TEST_METHOD(Production_creationEmptyProductionTest) {
		// Here we want it thrown an exception
		NonTerminal S("S");
		std::vector<Symbol> prod;
		bool expectionThrown;
		try {
			Production p(S, prod);
			expectionThrown = false;
		} catch (std::exception e) {
			expectionThrown = true;
		}
		// We should get an exception if the production is empty
		Assert::IsTrue(expectionThrown);
	};

	};
}