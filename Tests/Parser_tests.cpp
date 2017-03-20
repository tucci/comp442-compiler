#include "stdafx.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ParserTest) {
public:

	// Taken from http://stackoverflow.com/questions/15874723/how-to-open-a-file-from-the-project-in-a-native-c-unit-test-visual-studio
	// Needed in order to read test file in current directory
#define TEST_CASE_DIRECTORY GetDirectoryName(__FILE__)

	static std::string GetDirectoryName(std::string path) {
		const size_t last_slash_idx = path.rfind('\\');
		if (std::string::npos != last_slash_idx) {
			return path.substr(0, last_slash_idx + 1);
		}
		return "";
	}



	TEST_METHOD(Parser_parserFromFileTest1) {
		Compiler c(GetDirectoryName(TEST_CASE_DIRECTORY) + "grammar.txt", "prog", false);
		c.setSourceFile(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestSource1.txt");
		bool success = c.parsedSuccessfully;
		Assert::IsTrue(success);
	};


	TEST_METHOD(Parser_parserFromFileTest2) {
		Compiler c(GetDirectoryName(TEST_CASE_DIRECTORY) + "grammar.txt", "prog", false);
		c.setSourceFile(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestSource2.txt");
		bool success = c.parsedSuccessfully;
		Assert::IsTrue(success);
	};

	TEST_METHOD(Parser_parserFromFileTest3) {
		Compiler c(GetDirectoryName(TEST_CASE_DIRECTORY) + "grammar.txt", "prog", false);
		c.setSourceFile(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestSource3.txt");
		bool success = c.parsedSuccessfully;
		Assert::IsTrue(success);
	};

	TEST_METHOD(Parser_parserFromFileTest4) {
		// This test tests bad code
		Compiler c(GetDirectoryName(TEST_CASE_DIRECTORY) + "grammar.txt", "prog", false);
		c.setSourceFile(GetDirectoryName(TEST_CASE_DIRECTORY) + "TestSource4.txt");
		bool success = c.parsedSuccessfully;
		Assert::IsFalse(success);
	};
	
	};

	
}












