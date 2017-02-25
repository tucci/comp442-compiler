#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ProductionTest) {
public:

	TEST_METHOD(Production_errorProductionTest) {
		Assert::IsTrue("ERROR_PRODUCTION" == Production::ERROR_PRODUCTION_NON_TERMINAL.getName());
		Assert::IsTrue("ERROR_PRODUCTION" == Production::ERROR_PRODUCTION.getNonTerminal().getName());
		Assert::IsTrue(0 == Production::ERROR_PRODUCTION.getProduction().size());
	};


	TEST_METHOD(Production_emptyCreationTest) {
		NonTerminal S("");
		std::vector<Symbol> prod = {};
		try {
			Production p(S, prod);
			Assert::Fail();
		} catch (std::exception e) {

		}


	};

	TEST_METHOD(Production_creationNormalTest) {
		Terminal x("x");
		NonTerminal a("A");
		NonTerminal b("B");
		NonTerminal S("S");
		std::vector<Symbol> prod = { x,a,b };
		Production p(S, prod);
	};


	TEST_METHOD(Production_getNonTerminalTest) {
		Terminal x("x");
		NonTerminal a("A");
		NonTerminal b("B");
		NonTerminal S("S");
		std::vector<Symbol> prod = { x,a,b };
		Production p(S, prod);
		Assert::IsTrue(S.getName() == p.getNonTerminal().getName());
	};


	TEST_METHOD(Production_getRHSProductionTest) {
		Terminal x("x");
		NonTerminal a("A");
		NonTerminal b("B");
		NonTerminal S("S");
		std::vector<Symbol> prod = { x,a,b };
		Production p(S, prod);

		std::vector<Symbol> rhs = p.getProduction();
		Assert::IsTrue(prod.size() == rhs.size());
		for (int i = 0; i < prod.size(); i++) {
			Assert::IsTrue(prod.at(i).getName() == rhs.at(i).getName());
		}
	};


	TEST_METHOD(Production_productionEqualTest) {
		Terminal tx("x");
		Terminal tA("A");
		Terminal tB("B");
		Terminal tS("S");

		NonTerminal ntx("x");
		NonTerminal ntA("A");
		NonTerminal ntB("B");
		NonTerminal ntS("S");

		std::vector<Symbol> prod = { tx,ntA,ntB };
		
		Production p1(ntS, prod);
		Production p2(ntS, prod);
		
		Assert::IsTrue(p1 == p1);
		Assert::IsTrue(p1 == p2);
		Assert::IsTrue(p2 == p1);

		Production p3(ntB, prod);
		Assert::IsFalse(p1 == p3);
		Assert::IsFalse(p2 == p3);
		Assert::IsFalse(p3 == p1);
		Assert::IsFalse(p3 == p2);

		std::vector<Symbol> prod2 = { tx };
		Production p4(ntS, prod2);
		Assert::IsFalse(p1 == p4);

		// Same names but different symbol types
		std::vector<Symbol> prod3 = { ntx,tA,tB };
		Production p5(ntS, prod3);
		Assert::IsFalse(p1 == p5);
		
	};



	};
}