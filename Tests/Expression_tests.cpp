#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(ExpressionTest) {
public:


	TEST_METHOD(Expression_Test1) {
		Expression expr;
		expr.addNumeric("3");
		expr.buildExpressionTree();
		Assert::IsTrue("3" == expr.toFullName());
	};

	TEST_METHOD(Expression_Test2) {
		Expression expr;
		expr.addNumeric("3");
		expr.addOperator("+");
		expr.addNumeric("3");
		expr.buildExpressionTree();
		Assert::IsTrue("(3+3)" == expr.toFullName());
	};

	TEST_METHOD(Expression_Test3) {
		Expression expr;
		expr.addNumeric("54");
		expr.addOperator("*");
		expr.addNumeric("33");
		expr.buildExpressionTree();
		Assert::IsTrue("(54*33)" == expr.toFullName());
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
		expr.buildExpressionTree();
		Assert::IsTrue("(index+(3*index))" == expr.toFullName());
	};


	TEST_METHOD(Expression_TestTypeCheck1) {

		Expression expr;
		expr.addNumeric("3");
		expr.addOperator("+");
		expr.addNumeric("65");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_int;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};


	TEST_METHOD(Expression_TestTypeCheck2) {

		Expression expr;
		expr.addNumeric("3.4");
		expr.addOperator("+");
		expr.addNumeric("65.53");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_float;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck3) {
		// Type  mismatch
		Expression expr;
		expr.addNumeric("3");
		expr.addOperator("+");
		expr.addNumeric("65.53");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_mismatch;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(false, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck4) {
		

		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_int;
		var.varType.structure = struct_simple;
		
		
		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addNumeric("65");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_int;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};



	TEST_METHOD(Expression_TestTypeCheck5) {
		

		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_float;
		var.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addNumeric("65.45");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_float;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck6) {
		// Type  mismatch

		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_float;
		var.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addNumeric("65");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_mismatch;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(false, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck7) {
		// Type  mismatch

		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_int;
		var.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addNumeric("65.42");;
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_mismatch;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(false, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck8) {
	

		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_int;
		var.varType.structure = struct_simple;

		Variable var2;
		VariableFragment fragment2;
		fragment.identifier = "test2";
		var2.vars.push_back(fragment);
		var2.varType.type = type_int;
		var2.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addVar(var2);
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_int;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck9) {


		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_float;
		var.varType.structure = struct_simple;

		Variable var2;
		VariableFragment fragment2;
		fragment.identifier = "test2";
		var2.vars.push_back(fragment);
		var2.varType.type = type_float;
		var2.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addVar(var2);
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_float;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck10) {

		// type mismatch
		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_int;
		var.varType.structure = struct_simple;

		Variable var2;
		VariableFragment fragment2;
		fragment.identifier = "test2";
		var2.vars.push_back(fragment);
		var2.varType.type = type_float;
		var2.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addVar(var2);
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_mismatch;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(false, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};


	TEST_METHOD(Expression_TestTypeCheck11) {

		// type mismatch
		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_float;
		var.varType.structure = struct_simple;

		Variable var2;
		VariableFragment fragment2;
		fragment.identifier = "test2";
		var2.vars.push_back(fragment);
		var2.varType.type = type_int;
		var2.varType.structure = struct_simple;


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addVar(var2);
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_mismatch;
		expectedType.structure = struct_simple;
		std::pair<bool, TypeStruct> expected = std::make_pair(false, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};


	TEST_METHOD(Expression_TestTypeCheck12) {

		
		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_class;
		var.varType.structure = struct_simple;
		var.varType.className = "Test";

		Variable var2;
		VariableFragment fragment2;
		fragment.identifier = "test2";
		var2.vars.push_back(fragment);
		var2.varType.type = type_class;
		var2.varType.structure = struct_simple;
		var2.varType.className = "Test";


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addVar(var2);
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_class;
		expectedType.structure = struct_simple;
		expectedType.className = "Test";
		std::pair<bool, TypeStruct> expected = std::make_pair(true, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};

	TEST_METHOD(Expression_TestTypeCheck13) {

		// Type class int mismatch

		Variable var;
		VariableFragment fragment;
		fragment.identifier = "test";
		var.vars.push_back(fragment);
		var.varType.type = type_class;
		var.varType.structure = struct_simple;
		var.varType.className = "Test";

		Variable var2;
		VariableFragment fragment2;
		fragment.identifier = "test2";
		var2.vars.push_back(fragment);
		var2.varType.type = type_int;
		var2.varType.structure = struct_simple;
		


		Expression expr;
		expr.addVar(var);
		expr.addOperator("+");
		expr.addVar(var2);
		expr.buildExpressionTree();

		TypeStruct expectedType;
		expectedType.type = type_mismatch;
		expectedType.structure = struct_simple;
		
		std::pair<bool, TypeStruct> expected = std::make_pair(false, expectedType);
		std::pair<bool, TypeStruct> actual = expr.typeCheckExpression();
		Assert::IsTrue(expected.first == actual.first);
		Assert::IsTrue(expected.second == actual.second);
	};






	};
}