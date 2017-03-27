#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(SymbolTableDataTest) {
public:

	TEST_METHOD(SymbolTableData_symbolKindTests) {
		SymbolKind var = SymbolKind::kind_variable;
		SymbolKind param = SymbolKind::kind_parameter;
		SymbolKind func = SymbolKind::kind_function;
		SymbolKind classKind = SymbolKind::kind_class;

		Assert::IsTrue("kind_variable" == kindToString(var));
		Assert::IsTrue("kind_parameter" == kindToString(param));
		Assert::IsTrue("kind_function" == kindToString(func));
		Assert::IsTrue("kind_class" == kindToString(classKind));
	};


	TEST_METHOD(SymbolTableData_symbolTypeTests) {
		SymbolType intType = type_int;
		SymbolType floatType = type_float;
		SymbolType classType = type_class;

		Assert::IsTrue("type_int" == typeToString(intType));
		Assert::IsTrue("type_float" == typeToString(floatType));
		Assert::IsTrue("type_class" == typeToString(classType));
		
	};

	TEST_METHOD(SymbolTableData_symbolTypeStringToTypeTests) {
		Assert::IsTrue(type_int == stringToType(Specification::REVERSE_TOKEN_MAP.at(TokenType::int_token)));
		Assert::IsTrue(type_float == stringToType(Specification::REVERSE_TOKEN_MAP.at(TokenType::float_token)));
		Assert::IsTrue(type_class == stringToType("ClassName"));
	};


	TEST_METHOD(SymbolTableData_symbolStructureTests) {
		Assert::IsTrue("struct_simple" == structureToString(struct_simple));
		Assert::IsTrue("struct_array" == structureToString(struct_array));

	};







	};
}