#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(UtilsTest) {
public:

	TEST_METHOD(UtilsTest_ltrimTest) {
		std::string ls1 = "test";
		std::string ls2 = " test";
		std::string ls3 = "  test";
		std::string ls4 = "  t e s t";
		std::string ls5  = "test ";

		Assert::IsTrue("test" == ltrim(ls1));
		Assert::IsTrue("test" == ltrim(ls2));
		Assert::IsTrue("test" == ltrim(ls3));
		Assert::IsTrue("t e s t" == ltrim(ls4));
		Assert::IsTrue("test " == ltrim(ls5));
	};

	TEST_METHOD(UtilsTest_rtrimTest) {
		std::string rs1 = "test";
		std::string rs2 = "test ";
		std::string rs3 = "test  ";
		std::string rs4 = "t e s t  ";
		std::string rs5 = " test";

		Assert::IsTrue("test" == rtrim(rs1));
		Assert::IsTrue("test" == rtrim(rs2));
		Assert::IsTrue("test" == rtrim(rs3));
		Assert::IsTrue("t e s t" == rtrim(rs4));
		Assert::IsTrue(" test" == rtrim(rs5));
	};

	TEST_METHOD(UtilsTest_trimTest) {
		std::string s1 = "test";
		std::string s2 = " test ";
		std::string s3 = "  test  ";
		std::string s4 = "  t e s t  ";
		std::string s5 = " test ";

		Assert::IsTrue("test" == trim(s1));
		Assert::IsTrue("test" == trim(s2));
		Assert::IsTrue("test" == trim(s3));
		Assert::IsTrue("t e s t" == trim(s4));
		Assert::IsTrue("test" == trim(s5));
	};

	TEST_METHOD(UtilsTest_simpleSplitTest) {
		std::string s1 = "1 2 3 4";
		std::vector<std::string> s1vec = simpleSplit(s1);
		Assert::IsTrue("1" == s1vec.at(0));
		Assert::IsTrue("2" == s1vec.at(1));
		Assert::IsTrue("3" == s1vec.at(2));
		Assert::IsTrue("4" == s1vec.at(3));


		std::string s2 = "ab cd deee fgg";
		std::vector<std::string> s2vec = simpleSplit(s2);
		Assert::IsTrue("ab" == s2vec.at(0));
		Assert::IsTrue("cd" == s2vec.at(1));
		Assert::IsTrue("deee" == s2vec.at(2));
		Assert::IsTrue("fgg" == s2vec.at(3));
	}


	TEST_METHOD(UtilsTest_sublistTest) {
		std::vector<std::string> vec = {"a", "b", "c", "d"};

		std::vector<std::string> expected_subvec1 = sublist(vec, 0, 0);
		std::vector<std::string> expected_subvec2 = sublist(vec, 0, 1);
		std::vector<std::string> expected_subvec3 = sublist(vec, 0, 3);
		std::vector<std::string> expected_subvec4 = sublist(vec, 1, 3);
		std::vector<std::string> expected_subvec5 = sublist(vec, 3, 3);

		Assert::IsTrue(expected_subvec1.empty());

		Assert::IsTrue("a" == expected_subvec2.at(0));

		Assert::IsTrue("a" == expected_subvec3.at(0));
		Assert::IsTrue("b" == expected_subvec3.at(1));
		Assert::IsTrue("c" == expected_subvec3.at(2));

		Assert::IsTrue("b" == expected_subvec4.at(0));
		Assert::IsTrue("c" == expected_subvec4.at(1));

		Assert::IsTrue(expected_subvec5.empty());
	}


	TEST_METHOD(UtilsTest_inTerminalSetTest) {
		Terminal t1("a");
		Terminal t2("b");
		Terminal t3("c");
		Terminal t4("d");
		Terminal t5("e");

		Terminal t6("f");

		TerminalSet set = {t1, t2, t3, t4, t5, SpecialTerminal::END_OF_FILE, SpecialTerminal::EPSILON};
		Assert::IsTrue(inSet(t1, set));
		Assert::IsTrue(inSet(t2, set));
		Assert::IsTrue(inSet(t3, set));
		Assert::IsTrue(inSet(t4, set));
		Assert::IsTrue(inSet(t5, set));

		Assert::IsTrue(inSet(SpecialTerminal::END_OF_FILE, set));
		Assert::IsTrue(inSet(SpecialTerminal::EPSILON, set));

		Assert::IsFalse(inSet(t6, set));
	}


	TEST_METHOD(UtilsTest_inSetTest) {
		std::string a = "a";
		std::string b = "b";
		std::string c = "c";
		std::string d = "d";

		std::string e = "e";


		std::unordered_set<std::string> strSet = {"a", "b", "c", "d"};
		Assert::IsTrue(inSet(a, strSet));
		Assert::IsTrue(inSet(b, strSet));
		Assert::IsTrue(inSet(c, strSet));
		Assert::IsTrue(inSet(d, strSet));

		Assert::IsFalse(inSet(e, strSet));
	}


	TEST_METHOD(UtilsTest_inMapTest) {
		std::string a = "a";
		std::string b = "b";
		std::string c = "c";
		std::string d = "d";
		int aVal = 1;
		std::string e = "e";

		std::unordered_map<std::string, int> strMap;
		strMap.emplace(a, aVal);
		strMap.emplace(b, aVal);
		strMap.emplace(c, aVal);
		strMap.emplace(d, aVal);

		Assert::IsTrue(inMap(a, strMap));
		Assert::IsTrue(inMap(b, strMap));
		Assert::IsTrue(inMap(c, strMap));
		Assert::IsTrue(inMap(d, strMap));

		Assert::IsFalse(inMap(e, strMap));
	}

	TEST_METHOD(UtilsTest_leftMergeTest) {
		Terminal a("a");
		Terminal b("b");
		Terminal c("c");

		Terminal d("d");
		Terminal e("e");
		Terminal f("f");

		TerminalSet set1 = { a, b, c };
		TerminalSet set2 = { d, e, f };
		std::pair<TerminalSet, bool> actualSet = leftMerge(set1, set2);
		Assert::IsTrue(actualSet.second);
		Assert::IsTrue(inSet(a, actualSet.first));
		Assert::IsTrue(inSet(b, actualSet.first));
		Assert::IsTrue(inSet(c, actualSet.first));
		Assert::IsTrue(inSet(d, actualSet.first));
		Assert::IsTrue(inSet(e, actualSet.first));
		Assert::IsTrue(inSet(f, actualSet.first));

		TerminalSet set3 = { a };
		std::pair<TerminalSet, bool> actualSet2 = leftMerge(set1, set3);
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


