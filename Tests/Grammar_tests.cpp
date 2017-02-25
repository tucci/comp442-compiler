#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(GrammarTest) {
public:


	// Taken from http://stackoverflow.com/questions/15874723/how-to-open-a-file-from-the-project-in-a-native-c-unit-test-visual-studio
	// Needed in order to read test file in current directory
#define TEST_CASE_DIRECTORY GetDirectoryName(__FILE__)

	std::string GetDirectoryName(std::string path) {
		const size_t last_slash_idx = path.rfind('\\');
		if (std::string::npos != last_slash_idx) {
			return path.substr(0, last_slash_idx + 1);
		}
		return "";
	}

	TEST_METHOD(Grammar_createEmptyTest) {
		Grammar g;
		Assert::IsTrue(g.getNonTerminals().size() == 0);
		Assert::IsTrue(g.getTerminals().size() == 1); // 1 for $. symbol added on creation
		Assert::IsTrue(g.getProductions().size() == 0);
		Assert::IsTrue(g.getStartSymbol().getName() == "S");
	};

	TEST_METHOD(Grammar_createGrammarFromFileTest) {
		Grammar g(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestGrammarFile.txt", "E");
		Assert::IsTrue(g.getNonTerminals().size() == 5);
		Assert::IsTrue(g.getTerminals().size() == 6 + 1); // 1 for $. symbol added on creation
		Assert::IsTrue(g.getProductions().size() == 9);
		Assert::IsTrue(g.getStartSymbol().getName() == "E");

		Terminal t0("0");
		Terminal t1("1");
		Terminal tstart("*");
		Terminal tplus("+");
		Terminal topen("(");
		Terminal tclose(")");


		std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> terminals = g.getTerminals();
		// Since we get a map of pointers, we have to do some nasty checking
		for (auto t : terminals) {
			Assert::IsTrue(t->getName() == t0.getName()
				|| t->getName() == t1.getName()
				|| t->getName() == tstart.getName()
				|| t->getName() == tplus.getName()
				|| t->getName() == topen.getName()
				|| t->getName() == tclose.getName()
				|| t->getName() == SpecialTerminal::END_OF_FILE.getName()
				);
		}

		NonTerminal E("E");
		NonTerminal Ep("E'");
		NonTerminal T("T");
		NonTerminal Tp("T'");
		NonTerminal F("F");

		std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual> nonTerminals = g.getNonTerminals();
		// Since we get a map of pointers, we have to do some nasty checking
		for (auto nt : nonTerminals) {
			Assert::IsTrue(nt->getName() == E.getName()
				|| nt->getName() == Ep.getName()
				|| nt->getName() == T.getName()
				|| nt->getName() == Tp.getName()
				|| nt->getName() == F.getName()
				);
		}


	};

	TEST_METHOD(Grammar_addNonStartTerminalTest) {
		Grammar g;
		NonTerminal nt = g.addNonTerminal("E");
		Assert::IsTrue(g.getNonTerminals().size() == 1);
	};

	TEST_METHOD(Grammar_addTerminalTest) {
		Grammar g;
		Terminal t = g.addTerminal("a");
		Assert::IsTrue(g.getTerminals().size() == 1 + 1); // + 1 for $
	};

	TEST_METHOD(Grammar_addStartProductionTest) {
		Grammar g;
		NonTerminal start = g.addNonTerminal("start", true);
		Assert::IsTrue(g.getStartSymbol().getName() == start.getName());
	};

	TEST_METHOD(Grammar_addProductionTest) {
		Grammar g;
		NonTerminal start = g.addNonTerminal("start", true);
		Terminal a = g.addTerminal("a");
		NonTerminal B = g.addNonTerminal("B");
		std::vector<Symbol> prod = { a,B };
		g.addProduction(start, prod);

		Assert::IsTrue(g.getNonTerminals().size() == 2);
		Assert::IsTrue(g.getTerminals().size() == 1 + 1); // + 1 for $. symbol added on creation
		Assert::IsTrue(g.getProductions().size() == 1);
		Assert::IsTrue(g.getStartSymbol().getName() == start.getName());
	};

	TEST_METHOD(Grammar_getStartSymbolTest) {
		Grammar g;
		NonTerminal start = g.addNonTerminal("start", true);
		Assert::IsTrue(g.getStartSymbol().getName() == start.getName());
	};

	TEST_METHOD(Grammar_getProductionsTest) {
		Grammar g;
		NonTerminal start = g.addNonTerminal("start", true);
		Terminal a = g.addTerminal("a");
		NonTerminal B = g.addNonTerminal("B");
		std::vector<Symbol> prod = { a,B };

		Production p = g.addProduction(start, prod);
		Production p2 = g.addProduction(B, prod);

		Assert::IsTrue(p == *g.getProductions().at(0).get());
		Assert::IsTrue(p2 == *g.getProductions().at(1).get());

	};

	TEST_METHOD(Grammar_getTerminalsTest) {
		Grammar g;
		Terminal t1 = g.addTerminal("t1");
		Terminal t2 = g.addTerminal("t2");
		Terminal t3 = g.addTerminal("t3");
		Terminal t4 = g.addTerminal("t4");

		Assert::IsTrue(g.getTerminals().size() == 4 + 1); // + 1 for $. symbol added on creation

		std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> terminals = g.getTerminals();
		// Since we get a map of pointers, we have to do some nasty checking
		for (auto t : terminals) {
			Assert::IsTrue(t->getName() == t1.getName()
				|| t->getName() == t2.getName()
				|| t->getName() == t3.getName()
				|| t->getName() == t4.getName()
				|| t->getName() == SpecialTerminal::END_OF_FILE.getName());
		}


	};

	TEST_METHOD(Grammar_getNonTerminalsTest) {
		Grammar g;
		NonTerminal nt1 = g.addNonTerminal("nt1");
		NonTerminal nt2 = g.addNonTerminal("nt2");
		NonTerminal nt3 = g.addNonTerminal("nt3");
		NonTerminal nt4 = g.addNonTerminal("nt4");

		Assert::IsTrue(g.getNonTerminals().size() == 4); // doesnt include pre include start symbol

		std::unordered_set<std::shared_ptr<NonTerminal>, SymbolHasher, SymbolEqual> nonTerminals = g.getNonTerminals();
		// Since we get a map of pointers, we have to do some nasty checking
		for (auto nt : nonTerminals) {
			Assert::IsTrue(nt->getName() == nt1.getName()
				|| nt->getName() == nt2.getName()
				|| nt->getName() == nt3.getName()
				|| nt->getName() == nt4.getName()
				|| nt->getName() == g.getStartSymbol().getName());
		}
	};
	};

}






	




