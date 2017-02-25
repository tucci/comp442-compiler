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


	TEST_METHOD(FirstFollowSetGenerator_createGrammarFromFileTest) {
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

		NonTerminal E("E");
		NonTerminal Ep("E'");
		NonTerminal T("T");
		NonTerminal Tp("T'");
		NonTerminal F("F"););
		}


	};

	};
}











