#include "stdafx.h"
#include "CppUnitTest.h"

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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LexerTests {
	TEST_CLASS(LexerTest) {
public:

	static std::string testFile;
	static Specification spec;
	static Lexer lex;
	static std::vector<Token> tokens;
	
	
	TEST_CLASS_INITIALIZE(INIT_LEXER_TEST) {		
		lex.setSource(TEST_CASE_DIRECTORY + testFile);
		while (lex.hasMoreTokens()) {
			tokens.push_back(lex.nextToken());
		}
	}

	
	TEST_METHOD(lexerTestNoComments) {
		for (std::vector<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
			Assert::AreNotEqual(static_cast<int>(TokenType::cmt), static_cast<int>(it->type));
		}
	};

	};
	std::string LexerTest::testFile = "TestLexerSourceFile.txt";
	Specification LexerTest::spec(true);
	Lexer LexerTest::lex(&spec);
	std::vector<Token> LexerTest::tokens;
	


}