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

namespace Tests {
	TEST_CLASS(LexerTest) {
public:
	// Note the TestLexerSourceFile.txt should never be changed
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

	
	TEST_METHOD(Lexer_TestNoComments) {
		for (std::vector<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
			Assert::AreNotEqual(static_cast<int>(TokenType::cmt), static_cast<int>(it->type));
		}
		Assert::IsTrue(tokens.at(104).error.type == mulitcomment_error);
	};


	TEST_METHOD(Lexer_InvalidSymbolTest) {
		Assert::IsTrue(tokens.at(0).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(1).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(2).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(3).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(4).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(5).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(6).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(7).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(8).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(9).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(10).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(11).error.type == unknown_symbol);
		Assert::IsTrue(tokens.at(0).type == error_token && tokens.at(0).lexeme == "~" && tokens.at(0).tokenLine == 2);
		Assert::IsTrue(tokens.at(1).type == error_token && tokens.at(1).lexeme == "`" && tokens.at(1).tokenLine == 2);
		Assert::IsTrue(tokens.at(2).type == error_token && tokens.at(2).lexeme == "!" && tokens.at(2).tokenLine == 2);
		Assert::IsTrue(tokens.at(3).type == error_token && tokens.at(3).lexeme == "@" && tokens.at(3).tokenLine == 2);
		Assert::IsTrue(tokens.at(4).type == error_token && tokens.at(4).lexeme == "#" && tokens.at(4).tokenLine == 2);
		Assert::IsTrue(tokens.at(5).type == error_token && tokens.at(5).lexeme == "$" && tokens.at(5).tokenLine == 2);
		Assert::IsTrue(tokens.at(6).type == error_token && tokens.at(6).lexeme == "%" && tokens.at(6).tokenLine == 2);
		Assert::IsTrue(tokens.at(7).type == error_token && tokens.at(7).lexeme == "^" && tokens.at(7).tokenLine == 2);
		Assert::IsTrue(tokens.at(8).type == error_token && tokens.at(8).lexeme == "&" && tokens.at(8).tokenLine == 2);
		Assert::IsTrue(tokens.at(9).type == error_token && tokens.at(9).lexeme == "?" && tokens.at(9).tokenLine == 2);
		Assert::IsTrue(tokens.at(10).type == error_token && tokens.at(10).lexeme == "\\" && tokens.at(10).tokenLine == 2);
		Assert::IsTrue(tokens.at(11).type == error_token && tokens.at(11).lexeme == "|" && tokens.at(11).tokenLine == 2);

	};

	TEST_METHOD(Lexer_FloatErrorsAndRecovery) {
		// Recovery
		Assert::IsTrue(tokens.at(12).type == int_token && tokens.at(12).lexeme == "0" && tokens.at(12).tokenLine == 5);
		Assert::IsTrue(tokens.at(13).type == dot && tokens.at(13).lexeme == "." && tokens.at(13).tokenLine == 5);
		Assert::IsTrue(tokens.at(14).type == id && tokens.at(14).lexeme == "s" && tokens.at(14).tokenLine == 5);
		// invalid_float error
		Assert::IsTrue(tokens.at(15).type == error_token && tokens.at(15).lexeme == "1.00" && tokens.at(15).tokenLine == 6);
		Assert::IsTrue(tokens.at(15).error.type == invalid_float);

		Assert::IsTrue(tokens.at(16).type == num && tokens.at(16).lexeme == "2.0" && tokens.at(16).tokenLine == 7);
		Assert::IsTrue(tokens.at(17).type == int_token && tokens.at(17).lexeme == "3" && tokens.at(17).tokenLine == 8);
		Assert::IsTrue(tokens.at(18).type == dot && tokens.at(18).lexeme == "." && tokens.at(18).tokenLine == 8);
		Assert::IsTrue(tokens.at(19).type == id && tokens.at(19).lexeme == "a" && tokens.at(19).tokenLine == 8);
		Assert::IsTrue(tokens.at(20).type == num && tokens.at(20).lexeme == "4.2" && tokens.at(20).tokenLine == 9);
		Assert::IsTrue(tokens.at(21).type == id && tokens.at(21).lexeme == "a" && tokens.at(21).tokenLine == 9);
		Assert::IsTrue(tokens.at(22).type == num && tokens.at(22).lexeme == "5.0" && tokens.at(22).tokenLine == 10);
		Assert::IsTrue(tokens.at(23).type == id && tokens.at(23).lexeme == "a" && tokens.at(23).tokenLine == 10);
		Assert::IsTrue(tokens.at(24).type == num && tokens.at(24).lexeme == "6.03" && tokens.at(24).tokenLine == 11);
		Assert::IsTrue(tokens.at(25).type == id && tokens.at(25).lexeme == "a" && tokens.at(25).tokenLine == 11);
		Assert::IsTrue(tokens.at(26).type == num && tokens.at(26).lexeme == "7.00000000003" && tokens.at(26).tokenLine == 12);
		Assert::IsTrue(tokens.at(27).type == id && tokens.at(27).lexeme == "a" && tokens.at(27).tokenLine == 12);

		// invalid_float error
		Assert::IsTrue(tokens.at(28).type == error_token && tokens.at(28).lexeme == "8.30" && tokens.at(28).tokenLine == 13);
		Assert::IsTrue(tokens.at(28).error.type == invalid_float);

		Assert::IsTrue(tokens.at(29).type == int_token && tokens.at(29).lexeme == "0" && tokens.at(29).tokenLine == 14);
		Assert::IsTrue(tokens.at(30).type == dot && tokens.at(30).lexeme == "." && tokens.at(30).tokenLine == 14);
		Assert::IsTrue(tokens.at(31).type == id && tokens.at(31).lexeme == "a1" && tokens.at(31).tokenLine == 14);
		Assert::IsTrue(tokens.at(32).type == num && tokens.at(32).lexeme == "0.1" && tokens.at(32).tokenLine == 15);
		Assert::IsTrue(tokens.at(33).type == id && tokens.at(33).lexeme == "a" && tokens.at(33).tokenLine == 15);

		// invalid_float error
		Assert::IsTrue(tokens.at(34).type == error_token && tokens.at(34).lexeme == "1.00" && tokens.at(34).tokenLine == 16);
		Assert::IsTrue(tokens.at(34).error.type == invalid_float);

		Assert::IsTrue(tokens.at(35).type == id && tokens.at(35).lexeme == "a" && tokens.at(35).tokenLine == 16);
		Assert::IsTrue(tokens.at(36).type == num && tokens.at(36).lexeme == "2.0" && tokens.at(36).tokenLine == 17);
		Assert::IsTrue(tokens.at(37).type == id && tokens.at(37).lexeme == "a" && tokens.at(37).tokenLine == 17);
		Assert::IsTrue(tokens.at(38).type == num && tokens.at(38).lexeme == "3.003" && tokens.at(38).tokenLine == 18);
		Assert::IsTrue(tokens.at(39).type == id && tokens.at(39).lexeme == "a" && tokens.at(39).tokenLine == 18);

		// invalid_float error
		Assert::IsTrue(tokens.at(40).type == error_token && tokens.at(40).lexeme == "4.10" && tokens.at(40).tokenLine == 19);
		Assert::IsTrue(tokens.at(40).error.type == invalid_float);

		Assert::IsTrue(tokens.at(41).type == id && tokens.at(41).lexeme == "a" && tokens.at(41).tokenLine == 19);
	};

	TEST_METHOD(Lexer_Operators) {
		Assert::IsTrue(tokens.at(42).type == comparison && tokens.at(42).lexeme == "==" && tokens.at(42).tokenLine == 23);
		Assert::IsTrue(tokens.at(43).type == noteq && tokens.at(43).lexeme == "<>" && tokens.at(43).tokenLine == 24);
		Assert::IsTrue(tokens.at(44).type == lt && tokens.at(44).lexeme == "<" && tokens.at(44).tokenLine == 25);
		Assert::IsTrue(tokens.at(45).type == gt && tokens.at(45).lexeme == ">" && tokens.at(45).tokenLine == 26);
		Assert::IsTrue(tokens.at(46).type == lesseq && tokens.at(46).lexeme == "<=" && tokens.at(46).tokenLine == 27);
		Assert::IsTrue(tokens.at(47).type == greateq && tokens.at(47).lexeme == ">=" && tokens.at(47).tokenLine == 28);
		Assert::IsTrue(tokens.at(48).type == semicolon && tokens.at(48).lexeme == ";" && tokens.at(48).tokenLine == 29);
		Assert::IsTrue(tokens.at(49).type == comma && tokens.at(49).lexeme == "," && tokens.at(49).tokenLine == 30);
		Assert::IsTrue(tokens.at(50).type == dot && tokens.at(50).lexeme == "." && tokens.at(50).tokenLine == 31);
		Assert::IsTrue(tokens.at(51).type == addop && tokens.at(51).lexeme == "+" && tokens.at(51).tokenLine == 32);
		Assert::IsTrue(tokens.at(52).type == subtractop && tokens.at(52).lexeme == "-" && tokens.at(52).tokenLine == 33);
		Assert::IsTrue(tokens.at(53).type == multop && tokens.at(53).lexeme == "*" && tokens.at(53).tokenLine == 34);
		Assert::IsTrue(tokens.at(54).type == divop && tokens.at(54).lexeme == "/" && tokens.at(54).tokenLine == 35);
		Assert::IsTrue(tokens.at(55).type == assgn && tokens.at(55).lexeme == "=" && tokens.at(55).tokenLine == 36);
		Assert::IsTrue(tokens.at(56).type == and && tokens.at(56).lexeme == "and" && tokens.at(56).tokenLine == 37);
		Assert::IsTrue(tokens.at(57).type == id && tokens.at(57).lexeme == "and_" && tokens.at(57).tokenLine == 37);
		Assert::IsTrue(tokens.at(58).type == id && tokens.at(58).lexeme == "andtest" && tokens.at(58).tokenLine == 37);
		Assert::IsTrue(tokens.at(59).type == not && tokens.at(59).lexeme == "not" && tokens.at(59).tokenLine == 38);
		Assert::IsTrue(tokens.at(60).type == id && tokens.at(60).lexeme == "not_" && tokens.at(60).tokenLine == 38);
		Assert::IsTrue(tokens.at(61).type == id && tokens.at(61).lexeme == "nottest" && tokens.at(61).tokenLine == 38);
		Assert::IsTrue(tokens.at(62).type == or && tokens.at(62).lexeme == "or" && tokens.at(62).tokenLine == 39);
		Assert::IsTrue(tokens.at(63).type == id && tokens.at(63).lexeme == "or_" && tokens.at(63).tokenLine == 39);
		Assert::IsTrue(tokens.at(64).type == id && tokens.at(64).lexeme == "ortest" && tokens.at(64).tokenLine == 39);
		Assert::IsTrue(tokens.at(65).type == openpar && tokens.at(65).lexeme == "(" && tokens.at(65).tokenLine == 40);
		Assert::IsTrue(tokens.at(66).type == closepar && tokens.at(66).lexeme == ")" && tokens.at(66).tokenLine == 41);
		Assert::IsTrue(tokens.at(67).type == openbrace && tokens.at(67).lexeme == "{" && tokens.at(67).tokenLine == 42);
		Assert::IsTrue(tokens.at(68).type == closebrace && tokens.at(68).lexeme == "}" && tokens.at(68).tokenLine == 43);
		Assert::IsTrue(tokens.at(69).type == opensquare && tokens.at(69).lexeme == "[" && tokens.at(69).tokenLine == 44);
		Assert::IsTrue(tokens.at(70).type == closesquare && tokens.at(70).lexeme == "]" && tokens.at(70).tokenLine == 45);
	};

	TEST_METHOD(Lexer_ReservedWords) {
		Assert::IsTrue(tokens.at(71).type == if_token && tokens.at(71).lexeme == "if" && tokens.at(71).tokenLine == 47);
		Assert::IsTrue(tokens.at(72).type == id && tokens.at(72).lexeme == "if_" && tokens.at(72).tokenLine == 47);
		Assert::IsTrue(tokens.at(73).type == id && tokens.at(73).lexeme == "iftest" && tokens.at(73).tokenLine == 47);

		Assert::IsTrue(tokens.at(74).type == then_token && tokens.at(74).lexeme == "then" && tokens.at(74).tokenLine == 48);
		Assert::IsTrue(tokens.at(75).type == id && tokens.at(75).lexeme == "then_" && tokens.at(75).tokenLine == 48);
		Assert::IsTrue(tokens.at(76).type == id && tokens.at(76).lexeme == "thentest" && tokens.at(76).tokenLine == 48);

		Assert::IsTrue(tokens.at(77).type == else_token && tokens.at(77).lexeme == "else" && tokens.at(77).tokenLine == 49);
		Assert::IsTrue(tokens.at(78).type == id && tokens.at(78).lexeme == "else_" && tokens.at(78).tokenLine == 49);
		Assert::IsTrue(tokens.at(79).type == id && tokens.at(79).lexeme == "elsetest" && tokens.at(79).tokenLine == 49);

		Assert::IsTrue(tokens.at(80).type == for_token && tokens.at(80).lexeme == "for" && tokens.at(80).tokenLine == 50);
		Assert::IsTrue(tokens.at(81).type == id && tokens.at(81).lexeme == "for_" && tokens.at(81).tokenLine == 50);
		Assert::IsTrue(tokens.at(82).type == id && tokens.at(82).lexeme == "fortest" && tokens.at(82).tokenLine == 50);

		Assert::IsTrue(tokens.at(83).type == class_token && tokens.at(83).lexeme == "class" && tokens.at(83).tokenLine == 51);
		Assert::IsTrue(tokens.at(84).type == id && tokens.at(84).lexeme == "class_" && tokens.at(84).tokenLine == 51);
		Assert::IsTrue(tokens.at(85).type == id && tokens.at(85).lexeme == "classtest" && tokens.at(85).tokenLine == 51);

		Assert::IsTrue(tokens.at(86).type == int_token && tokens.at(86).lexeme == "int" && tokens.at(86).tokenLine == 52);
		Assert::IsTrue(tokens.at(87).type == id && tokens.at(87).lexeme == "int_" && tokens.at(87).tokenLine == 52);
		Assert::IsTrue(tokens.at(88).type == id && tokens.at(88).lexeme == "inttest" && tokens.at(88).tokenLine == 52);

		Assert::IsTrue(tokens.at(89).type == float_token && tokens.at(89).lexeme == "float" && tokens.at(89).tokenLine == 53);
		Assert::IsTrue(tokens.at(90).type == id && tokens.at(90).lexeme == "float_" && tokens.at(90).tokenLine == 53);
		Assert::IsTrue(tokens.at(91).type == id && tokens.at(91).lexeme == "floattest" && tokens.at(91).tokenLine == 53);

		Assert::IsTrue(tokens.at(92).type == get_token && tokens.at(92).lexeme == "get" && tokens.at(92).tokenLine == 54);
		Assert::IsTrue(tokens.at(93).type == id && tokens.at(93).lexeme == "get_" && tokens.at(93).tokenLine == 54);
		Assert::IsTrue(tokens.at(94).type == id && tokens.at(94).lexeme == "gettest" && tokens.at(94).tokenLine == 54);

		Assert::IsTrue(tokens.at(95).type == put_token && tokens.at(95).lexeme == "put" && tokens.at(95).tokenLine == 55);
		Assert::IsTrue(tokens.at(96).type == id && tokens.at(96).lexeme == "put_" && tokens.at(96).tokenLine == 55);
		Assert::IsTrue(tokens.at(97).type == id && tokens.at(97).lexeme == "puttest" && tokens.at(97).tokenLine == 55);

		Assert::IsTrue(tokens.at(98).type == return_token && tokens.at(98).lexeme == "return" && tokens.at(98).tokenLine == 56);
		Assert::IsTrue(tokens.at(99).type == id && tokens.at(99).lexeme == "return_" && tokens.at(99).tokenLine == 56);
		Assert::IsTrue(tokens.at(100).type == id && tokens.at(100).lexeme == "returntest" && tokens.at(100).tokenLine == 56);

		Assert::IsTrue(tokens.at(101).type == program_token && tokens.at(101).lexeme == "program" && tokens.at(101).tokenLine == 57);
		Assert::IsTrue(tokens.at(102).type == id && tokens.at(102).lexeme == "program_" && tokens.at(102).tokenLine == 57);
		Assert::IsTrue(tokens.at(103).type == id && tokens.at(103).lexeme == "programtest" && tokens.at(103).tokenLine == 57);
	};

	};
	std::string LexerTest::testFile = "TestLexerSourceFile.txt";
	Specification LexerTest::spec(true);
	Lexer LexerTest::lex(&spec);
	std::vector<Token> LexerTest::tokens;
	


}