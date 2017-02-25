#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(FirstFollowSetGeneratorTest) {
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

	TEST_METHOD(FirstFollowSetGenerator_initMapFileTest) {
		Grammar g(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestGrammarFile.txt", "E");
		NonTerminalMapToTerminalSet emptyMap = FirstFollowSetGenerator::initMap(g);

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

	TEST_METHOD(FirstFollowSetGenerator_firstfollowFileTest) {
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
		FST(E�) : { EPSILON,+ }
		FST(T) 	: { 0,1,( }	
		FST(T�) : { EPSILON,* }
		FST(F) 	: { 0,1,( }

		FLW(E)	: { $,) }
		FLW(E�) : { $,) }
		FLW(T) 	: { +,$,) }
		FLW(T�) : { +,$,) }
		FLW(F) 	: { *,+,$,) }
		*/

		NonTerminalMapToTerminalSet firstSet = FirstFollowSetGenerator::buildFirstSet(g);
		NonTerminalMapToTerminalSet followSet = FirstFollowSetGenerator::buildFollowSet(g, firstSet);

		// FST(E)	: { 0,1,( }
		TerminalSet firstE = firstSet.at(E);
		Assert::IsTrue(inSet(t0, firstE));
		Assert::IsTrue(inSet(t1, firstE));
		Assert::IsTrue(inSet(topen, firstE));

		// FST(E�) : { EPSILON,+ }
		TerminalSet firstEPrime = firstSet.at(Ep);
		Assert::IsTrue(inSet(SpecialTerminal::EPSILON, firstEPrime));
		Assert::IsTrue(inSet(tplus, firstEPrime));
		
		// FST(T) : { 0, 1, (}
		TerminalSet firstT = firstSet.at(T);
		Assert::IsTrue(inSet(t0, firstT));
		Assert::IsTrue(inSet(t1, firstT));
		Assert::IsTrue(inSet(topen, firstT));

		// FST(T�) : { EPSILON,* }
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

		// FLW(E�) : { $,) }
		TerminalSet followEPrime = followSet.at(Ep);
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followEPrime));
		Assert::IsTrue(inSet(tclose, followEPrime));

		// FLW(T) : { +, $, ) }
		TerminalSet followT = followSet.at(T);
		Assert::IsTrue(inSet(tplus, followT));
		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, followT));
		Assert::IsTrue(inSet(tclose, followT));

		// FLW(T�) : { +,$,) }
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

	TEST_METHOD(FirstFollowSetGenerator_leftMergeTest) {
		Terminal a("a");
		Terminal b("b");
		Terminal c("c");

		Terminal d("d");
		Terminal e("e");
		Terminal f("f");

		TerminalSet set1 = { a, b, c };
		TerminalSet set2 = { d, e, f };
		std::pair<TerminalSet, bool> actualSet = FirstFollowSetGenerator::leftMerge(set1, set2);
		Assert::IsTrue(actualSet.second);
		Assert::IsTrue(inSet(a, actualSet.first));
		Assert::IsTrue(inSet(b, actualSet.first));
		Assert::IsTrue(inSet(c, actualSet.first));
		Assert::IsTrue(inSet(d, actualSet.first));
		Assert::IsTrue(inSet(e, actualSet.first));
		Assert::IsTrue(inSet(f, actualSet.first));

		TerminalSet set3 = { a };
		std::pair<TerminalSet, bool> actualSet2 = FirstFollowSetGenerator::leftMerge(set1, set3);
		// Bool should be false because it shouldnt have changed anything when adding
		Assert::IsFalse(actualSet2.second);
		// The count of a should be 1
		Assert::IsTrue(actualSet2.first.count(a) == 1);
		Assert::IsTrue(inSet(a, actualSet2.first));
		Assert::IsTrue(inSet(b, actualSet2.first));
		Assert::IsTrue(inSet(c, actualSet2.first));
		Assert::IsFalse(inSet(d, actualSet2.first));
		Assert::IsFalse(inSet(e, actualSet2.first));
		Assert::IsFalse(inSet(f, actualSet2.first));
		
	};


};
}












