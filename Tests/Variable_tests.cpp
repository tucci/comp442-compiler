#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(VariableTest) {
public:

	TEST_METHOD(Variable_addFragment1Test) {
		Variable var;
		VariableFragment fragment{ "abc" };
		var.vars.push_back(fragment);
		Assert::IsTrue("abc" == var.toFullName());
	};


	TEST_METHOD(Variable_addFragment2Test) {
		Variable var;
		VariableFragment fragment{"abc"};
		VariableFragment fragment2{ "def" };
		var.vars.push_back(fragment);
		var.vars.push_back(fragment2);
		Assert::IsTrue("abc.def" == var.toFullName());
	};


	TEST_METHOD(Variable_addFragment3Test) {
		Variable var;
		VariableFragment fragment{ "abc" };
		VariableFragment fragment2{ "def" };
		VariableFragment fragment3{ "hij" };
		var.vars.push_back(fragment);
		var.vars.push_back(fragment2);
		var.vars.push_back(fragment3);
		Assert::IsTrue("abc.def.hij" == var.toFullName());
	};


	TEST_METHOD(Variable_addFragment4Test) {
		Variable var;
		Expression expr;
		expr.addNumeric("3");
		expr.buildExpressionTree();
		VariableFragment fragment{ "abc" };
		fragment.indices.push_back(expr);
		var.vars.push_back(fragment);
		Assert::IsTrue("abc[3]" == var.toFullName());
	};



	TEST_METHOD(Variable_addFragment5Test) {
		Variable varExpr;
		VariableFragment fragmentIndex{ "index" };
		varExpr.vars.push_back(fragmentIndex);
		Expression expr;
		expr.addVar(varExpr);
		expr.buildExpressionTree();
		Variable var;
		VariableFragment fragment{ "abc" };
		fragment.indices.push_back(expr);
		var.vars.push_back(fragment);
		Assert::IsTrue("abc[index]" == var.toFullName());
	};


	TEST_METHOD(Variable_addFragment6Test) {
		Variable varExpr;

		VariableFragment fragmentIndex{ "index" };
		varExpr.vars.push_back(fragmentIndex);

		Expression expr;
		expr.addVar(varExpr);
		expr.addOperator("+");
		expr.addNumeric("3");
		expr.addOperator("*");
		expr.addVar(varExpr);
		expr.buildExpressionTree();

		Variable var;
		VariableFragment fragment{ "abc" };
		fragment.indices.push_back(expr);
		var.vars.push_back(fragment);
		// No spaces when outputting
		// If you get an error here in the future, that means u changed the spacing
		Assert::IsTrue("abc[(index+(3*index))]" == var.toFullName());
	};

	TEST_METHOD(Variable_addFragment7Test) {
		Variable varExpr;
		VariableFragment fragmentIndex{ "index" };
		varExpr.vars.push_back(fragmentIndex);
		Expression expr;
		expr.addVar(varExpr);
		expr.addOperator("+");
		expr.addNumeric("3");
		expr.addOperator("*");
		expr.addVar(varExpr);
		expr.buildExpressionTree();
		Variable var;
		VariableFragment fragment{ "abc" };
		fragment.indices.push_back(expr);
		var.vars.push_back(fragment);

		VariableFragment fragment2{ "value" };
		var.vars.push_back(fragment2);
		// No spaces when outputting
		// If you get an error here in the future, that means u changed the spacing
		Assert::IsTrue("abc[(index+(3*index))].value" == var.toFullName());
	};


	TEST_METHOD(Variable_addFragment8Test) {
		Variable varExpr;
		VariableFragment fragmentIndex{ "index" };
		varExpr.vars.push_back(fragmentIndex);
		Expression expr;
		expr.addVar(varExpr);
		expr.addOperator("+");
		expr.addNumeric("3");
		expr.addOperator("*");
		expr.addVar(varExpr);
		expr.buildExpressionTree();
		Variable var;
		VariableFragment fragment{ "abc" };
		fragment.indices.push_back(expr);
		var.vars.push_back(fragment);

		VariableFragment fragment2{ "value" };
		var.vars.push_back(fragment2);
		var.isFuncCall = true;
		// No spaces when outputting
		// If you get an error here in the future, that means u changed the spacing
		Assert::IsTrue("abc[(index+(3*index))].value()" == var.toFullName());
	};



	TEST_METHOD(Variable_addFragment9Test) {
		Variable varExpr;
		VariableFragment fragmentIndex{ "index" };
		varExpr.vars.push_back(fragmentIndex);
		Expression expr;
		expr.addVar(varExpr);
		expr.addOperator("+");
		expr.addNumeric("3");
		expr.addOperator("*");
		expr.addVar(varExpr);
		expr.buildExpressionTree();
		Variable var;
		VariableFragment fragment{ "abc" };
		fragment.indices.push_back(expr);
		var.vars.push_back(fragment);

		VariableFragment fragment2{ "value" };
		var.vars.push_back(fragment2);
		var.isFuncCall = true;

		Expression arg1;
		Expression arg2;

		arg1.addNumeric("3");
		arg1.addOperator("+");
		arg1.addVar(varExpr);

		arg2.addVar(varExpr);
		arg2.addOperator("*");
		arg2.addNumeric("9");

		arg1.buildExpressionTree();
		arg2.buildExpressionTree();
		var.arguments.push_back(arg1);
		var.arguments.push_back(arg2);
		// No spaces when outputting
		// If you get an error here in the future, that means u changed the spacing
		Assert::IsTrue("abc[(index+(3*index))].value((3+index), (index*9), )" == var.toFullName());
	};









	};
}