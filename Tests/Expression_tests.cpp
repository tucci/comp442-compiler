#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ExpressionTest) {
public:


	TEST_METHOD(Expression_Test1) {
		Expression expr;
		expr.addNumeric("3");
		Assert::IsTrue("3" == expr.toFullName());
	};

	TEST_METHOD(Expression_Test2) {
		Expression expr;
		expr.addNumeric("3");
		expr.addOperator("+");
		expr.addNumeric("3");
		Assert::IsTrue("3+3" == expr.toFullName());
	};

	TEST_METHOD(Expression_Test3) {
		Expression expr;
		expr.addNumeric("54");
		expr.addOperator("*");
		expr.addNumeric("33");
		Assert::IsTrue("54*33" == expr.toFullName());
	};

	TEST_METHOD(Expression_Test4) {
		Variable varExpr;
		VariableFragment fragmentIndex{ "index" };
		varExpr.vars.push_back(fragmentIndex);
		Expression expr;
		expr.addVar(varExpr);
		expr.addOperator("+");
		expr.addNumeric("3");
		expr.addOperator("*");
		expr.addVar(varExpr);

		Assert::IsTrue("index+3*index" == expr.toFullName());

		
	};









	};
}