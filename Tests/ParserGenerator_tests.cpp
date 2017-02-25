#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ParserGeneratorTest) {
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

	TEST_METHOD(ParserGeneratorTest_initMapFileTest) {
		Grammar g(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestGrammarFile.txt", "E");
		std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> emptyMap = ParserGenerator::initMap(g);

		NonTerminal E("E");
		NonTerminal Ep("E'");
		NonTerminal T("T");
		NonTerminal Tp("T'");
		NonTerminal F("F");

		// Not supposed to be in there
		NonTerminal N("N");

		Assert::IsTrue(inMap(E, emptyMap));
		Assert::IsTrue(inMap(Ep, emptyMap));
		Assert::IsTrue(inMap(T, emptyMap));
		Assert::IsTrue(inMap(Tp, emptyMap));
		Assert::IsTrue(inMap(F, emptyMap));

		// Not supposed to be in there
		Assert::IsFalse(inMap(N, emptyMap));

	};

	TEST_METHOD(ParserGeneratorTest_buildfirstfollowFileTest) {
		Grammar g(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestGrammarFile.txt", "E");
		Assert::IsTrue(g.getNonTerminals().size() == 5);
		Assert::IsTrue(g.getTerminals().size() == 6 + 1); // 1 for $. symbol added on creation
		Assert::IsTrue(g.getProductions().size() == 9);
		Assert::IsTrue(g.getStartSymbol().getName() == "E");

		Terminal t0("0");
		Terminal t1("1");
		Terminal tstar("*");
		Terminal tplus("+");
		Terminal topen("(");
		Terminal tclose(")");

		NonTerminal E("E");
		NonTerminal Ep("E'");
		NonTerminal T("T");
		NonTerminal Tp("T'");
		NonTerminal F("F");

		/*	
		// Test to see if this grammar has these sets
		FST(E)	: { 0,1,( }
		FST(E’) : { EPSILON,+ }
		FST(T) 	: { 0,1,( }	
		FST(T’) : { EPSILON,* }
		FST(F) 	: { 0,1,( }

		FLW(E)	: { $,) }
		FLW(E’) : { $,) }
		FLW(T) 	: { +,$,) }
		FLW(T’) : { +,$,) }
		FLW(F) 	: { *,+,$,) }
		*/

		std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> firstSet = ParserGenerator::buildFirstSet(g);
		std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> followSet = ParserGenerator::buildFollowSet(g, firstSet);

		// FST(E)	: { 0,1,( }
		TerminalSet firstE = firstSet.at(E);
		Assert::IsTrue(inSet(t0, firstE));
		Assert::IsTrue(inSet(t1, firstE));
		Assert::IsTrue(inSet(topen, firstE));

		// FST(E’) : { EPSILON,+ }
		TerminalSet firstEPrime = firstSet.at(Ep);
		Assert::IsTrue(inSet(SpecialTerminal::EPSILON, firstEPrime));
		Assert::IsTrue(inSet(tplus, firstEPrime));
		
		// FST(T) : { 0, 1, (}
		TerminalSet firstT = firstSet.at(T);
		Assert::IsTrue(inSet(t0, firstT));
		Assert::IsTrue(inSet(t1, firstT));
		Assert::IsTrue(inSet(topen, firstT));

		// FST(T’) : { EPSILON,* }
		TerminalSet firstTPrime = firstSet.at(Tp);
		Assert::IsTrue(inSet(SpecialTerminal::EPSILON, firstTPrime));
		Assert::IsTrue(inSet(tstar, firstTPrime));

		// FST(F) 	: { 0,1,( }
		TerminalSet firstF = firstSet.at(F);
		Assert::IsTrue(inSet(t0, firstF));
		Assert::IsTrue(inSet(t1, firstF));
		Assert::IsTrue(inSet(topen, firstF));

		// FLW(E)	: { $,) }
		TerminalSet followE = followSet.at(E);
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followE));
		Assert::IsTrue(inSet(tclose, followE));

		// FLW(E’) : { $,) }
		TerminalSet followEPrime = followSet.at(Ep);
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followEPrime));
		Assert::IsTrue(inSet(tclose, followEPrime));

		// FLW(T) : { +, $, ) }
		TerminalSet followT = followSet.at(T);
		Assert::IsTrue(inSet(tplus, followT));
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followT));
		Assert::IsTrue(inSet(tclose, followT));

		// FLW(T’) : { +,$,) }
		TerminalSet followTPrime = followSet.at(Tp);
		Assert::IsTrue(inSet(tplus, followTPrime));
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followTPrime));
		Assert::IsTrue(inSet(tclose, followTPrime));

		// FLW(F) 	: { *,+,$,) }
		TerminalSet followF = followSet.at(F);
		Assert::IsTrue(inSet(tstar, followF));
		Assert::IsTrue(inSet(tplus, followF));
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followF));
		Assert::IsTrue(inSet(tclose, followF));
	};

	TEST_METHOD(ParserGeneratorTest_parserTableFileTest) {
		Grammar g(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestGrammarFile.txt", "E");
		Assert::IsTrue(g.getNonTerminals().size() == 5);
		Assert::IsTrue(g.getTerminals().size() == 6 + 1); // 1 for $. symbol added on creation
		Assert::IsTrue(g.getProductions().size() == 9);
		Assert::IsTrue(g.getStartSymbol().getName() == "E");

		Terminal t0("0");
		Terminal t1("1");
		Terminal tstar("*");
		Terminal tplus("+");
		Terminal topen("(");
		Terminal tclose(")");

		NonTerminal E("E");
		NonTerminal Ep("E'");
		NonTerminal T("T");
		NonTerminal Tp("T'");
		NonTerminal F("F");

		Production r1(E, {T, Ep});
		Production r2(Ep, { tplus, T, Ep });
		Production r3(Ep, { SpecialTerminal::EPSILON });
		Production r4(T, { F, Tp });
		Production r5(Tp, { tstar, F, Tp });
		Production r6(Tp, { SpecialTerminal::EPSILON });

		Production r7(F, { t0 });
		Production r8(F, { t1});
		Production r9(F, { topen, E, tclose});


		std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> parseTable = ParserGenerator::buildParseTable(g);

		// Row E
		Assert::IsTrue(parseTable.at(E).at(t0) == r1);
		Assert::IsTrue(parseTable.at(E).at(t1) == r1);
		Assert::IsTrue(parseTable.at(E).at(topen) == r1);
		Assert::IsTrue(parseTable.at(E).at(tclose) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(E).at(tplus) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(E).at(tstar) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(E).at(SpecialTerminal::END_OF_FILE) == Production::ERROR_PRODUCTION);

		// Row E'
		Assert::IsTrue(parseTable.at(Ep).at(t0) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Ep).at(t1) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Ep).at(topen) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Ep).at(tclose) == r3);
		Assert::IsTrue(parseTable.at(Ep).at(tplus) == r2);
		Assert::IsTrue(parseTable.at(Ep).at(tstar) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Ep).at(SpecialTerminal::END_OF_FILE) == r3);

		// Row T
		Assert::IsTrue(parseTable.at(T).at(t0) == r4);
		Assert::IsTrue(parseTable.at(T).at(t1) == r4);
		Assert::IsTrue(parseTable.at(T).at(topen) == r4);
		Assert::IsTrue(parseTable.at(T).at(tclose) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(T).at(tplus) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(T).at(tstar) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(T).at(SpecialTerminal::END_OF_FILE) == Production::ERROR_PRODUCTION);

		// Row T'
		Assert::IsTrue(parseTable.at(Tp).at(t0) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Tp).at(t1) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Tp).at(topen) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(Tp).at(tclose) == r6);
		Assert::IsTrue(parseTable.at(Tp).at(tplus) == r6);
		Assert::IsTrue(parseTable.at(Tp).at(tstar) == r5);
		Assert::IsTrue(parseTable.at(Tp).at(SpecialTerminal::END_OF_FILE) == r6);

		// Row F
		Assert::IsTrue(parseTable.at(F).at(t0) == r7);
		Assert::IsTrue(parseTable.at(F).at(t1) == r8);
		Assert::IsTrue(parseTable.at(F).at(topen) == r9);
		Assert::IsTrue(parseTable.at(F).at(tclose) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(F).at(tplus) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(F).at(tstar) == Production::ERROR_PRODUCTION);
		Assert::IsTrue(parseTable.at(F).at(SpecialTerminal::END_OF_FILE) == Production::ERROR_PRODUCTION);
	};


	TEST_METHOD(ParserGeneratorTest_infirstfollowFileTest) {
		typedef ParserGenerator pg;
		Grammar g(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestGrammarFile.txt", "E");
		Assert::IsTrue(g.getNonTerminals().size() == 5);
		Assert::IsTrue(g.getTerminals().size() == 6 + 1); // 1 for $. symbol added on creation
		Assert::IsTrue(g.getProductions().size() == 9);
		Assert::IsTrue(g.getStartSymbol().getName() == "E");

		Terminal t0("0");
		Terminal t1("1");
		Terminal tstar("*");
		Terminal tplus("+");
		Terminal topen("(");
		Terminal tclose(")");

		NonTerminal E("E");
		NonTerminal Ep("E'");
		NonTerminal T("T");
		NonTerminal Tp("T'");
		NonTerminal F("F");

		/*
		// Test to see if this grammar has these sets
		FST(E)	: { 0,1,( }
		FST(E’) : { EPSILON,+ }
		FST(T) 	: { 0,1,( }
		FST(T’) : { EPSILON,* }
		FST(F) 	: { 0,1,( }

		FLW(E)	: { $,) }
		FLW(E’) : { $,) }
		FLW(T) 	: { +,$,) }
		FLW(T’) : { +,$,) }
		FLW(F) 	: { *,+,$,) }
		*/

		std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> firstSet = ParserGenerator::buildFirstSet(g);
		std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> followSet = ParserGenerator::buildFollowSet(g, firstSet);

		// FST(E)	: { 0,1,( }		
		Assert::IsTrue(pg::inFirst(t0, E, firstSet));
		Assert::IsTrue(pg::inFirst(t1, E, firstSet));
		Assert::IsTrue(pg::inFirst(topen, E, firstSet));

		// FST(E’) : { EPSILON,+ }		
		Assert::IsTrue(pg::inFirst(SpecialTerminal::EPSILON, Ep, firstSet));
		Assert::IsTrue(pg::inFirst(tplus, Ep, firstSet));

		// FST(T) : { 0, 1, (}		
		Assert::IsTrue(pg::inFirst(t0, T, firstSet));
		Assert::IsTrue(pg::inFirst(t1, T, firstSet));
		Assert::IsTrue(pg::inFirst(topen, T, firstSet));

		// FST(T’) : { EPSILON,* }		
		Assert::IsTrue(pg::inFirst(SpecialTerminal::EPSILON, Tp, firstSet));
		Assert::IsTrue(pg::inFirst(tstar, Tp, firstSet));

		// FST(F) 	: { 0,1,( }		
		Assert::IsTrue(pg::inFirst(t0, F, firstSet));
		Assert::IsTrue(pg::inFirst(t1, F, firstSet));
		Assert::IsTrue(pg::inFirst(topen, F, firstSet));

		// FLW(E)	: { $,) }		
		Assert::IsTrue(pg::inFollow(SpecialTerminal::END_OF_FILE, E, followSet));
		Assert::IsTrue(pg::inFollow(tclose, E, followSet));

		// FLW(E’) : { $,) }		
		Assert::IsTrue(pg::inFollow(SpecialTerminal::END_OF_FILE, Ep, followSet));
		Assert::IsTrue(pg::inFollow(tclose, Ep, followSet));

		// FLW(T) : { +, $, ) }	
		Assert::IsTrue(pg::inFollow(tplus, T, followSet));
		Assert::IsTrue(pg::inFollow(SpecialTerminal::END_OF_FILE, T, followSet));
		Assert::IsTrue(pg::inFollow(tclose, T, followSet));

		// FLW(T’) : { +,$,) }
		Assert::IsTrue(pg::inFollow(tplus, Tp, followSet));
		Assert::IsTrue(pg::inFollow(SpecialTerminal::END_OF_FILE, Tp, followSet));
		Assert::IsTrue(pg::inFollow(tclose, Tp, followSet));

		// FLW(F) 	: { *,+,$,) }
		Assert::IsTrue(pg::inFollow(tstar, F, followSet));
		Assert::IsTrue(pg::inFollow(tplus, F, followSet));
		Assert::IsTrue(pg::inFollow(SpecialTerminal::END_OF_FILE, F, followSet));
		Assert::IsTrue(pg::inFollow(tclose, F, followSet));
	};


};
}












