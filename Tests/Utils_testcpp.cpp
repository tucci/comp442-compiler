#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(UtilsTest) {
public:

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


