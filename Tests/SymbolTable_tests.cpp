#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(SymbolTableTest) {
public:

	TEST_METHOD(SymbolTable_constructorTest) {
		SymbolTable global;
		Assert::IsNull(global.parent);
	};

	TEST_METHOD(SymbolTable_parentTest) {
		SymbolTable global;
		SymbolTable child;
		child.parent = &global;
		Assert::IsNotNull(child.parent);
	};


	TEST_METHOD(SymbolTable_findTest) {
		SymbolTable global;
		SymbolTableRecord record;
		record.name = "testVar";
		record.scope = NULL; // no scope
		SymbolTableRecord* ptr = global.addRecord(record.name, record, NULL, false);
		std::pair<SymbolTableRecord*, bool> found = global.find(record.name);
		Assert::IsTrue(found.second);
	};

	TEST_METHOD(SymbolTable_find2Test) {
		SymbolTable global;
		SymbolTableRecord record;
		record.name = "testVar";
		record.scope = NULL; // no scope
		// Test not in there
		std::pair<SymbolTableRecord*, bool> found = global.find(record.name);
		Assert::IsFalse(found.second);
	};


	TEST_METHOD(SymbolTable_findinParentTest) {
		SymbolTable global;
		SymbolTable classTable;
		classTable.parent = &global;
		SymbolTableRecord record;
		record.name = "testVar";
		SymbolTableRecord* ptr = global.addRecord(record.name, record, NULL, false);
		std::pair<SymbolTableRecord*, bool> found = classTable.findInParents(record.name);
		Assert::IsTrue(found.second);
	};

	TEST_METHOD(SymbolTable_findinParen2Test) {
		SymbolTable global;
		SymbolTable classTable;
		classTable.parent = &global;
		SymbolTableRecord record;
		record.name = "testVar";
		//SymbolTableRecord* ptr = global.addRecord(record.name, record);
		std::pair<SymbolTableRecord*, bool> found = classTable.findInParents(record.name);
		Assert::IsFalse(found.second);

	};

	TEST_METHOD(SymbolTable_addRecordTest) {
		SymbolTable global;
		SymbolTableRecord record;
		record.properlyDeclared = true;
		record.name = "testVar";
		record.kind = SymbolKind::kind_variable;
		record.typeStructure = { SymbolType::type_int, SymbolStructure::struct_simple, "", {} };
		record.functionData = {};// none
		record.scope = NULL; // no scope
		record.address = 0; // no address now
		SymbolTableRecord* ptr = global.addRecord(record.name, record, NULL, false);
		Assert::IsNotNull(ptr);

	};

	TEST_METHOD(SymbolTable_addRecord2Test) {
		SymbolTable global;
		SymbolTableRecord record;
		record.properlyDeclared = true;
		record.name = "testVar";
		record.kind = SymbolKind::kind_variable;
		record.typeStructure = { SymbolType::type_int, SymbolStructure::struct_simple, "",{} };
		record.functionData = {};// none
		record.scope = NULL; // no scope
		record.address = 0; // no address now
		SymbolTableRecord* ptr = global.addRecord(record.name, record, NULL, false);
		SymbolTableRecord* ptr2 = global.addRecord(record.name, record, NULL, false);
		Assert::IsNotNull(ptr);
		Assert::IsNull(ptr2);
	};


	TEST_METHOD(SymbolTable_withoutScopeTest) {
		SymbolTable global;
		global.name = "Global";
		SymbolTableRecord record;
		record.name = "Test";
		// Third param is not specified
		SymbolTableRecord* ptr = global.addRecord(record.name, record, NULL, false);
		Assert::IsNull(ptr->scope.get());
	};

	TEST_METHOD(SymbolTable_withScopeTest) {
		SymbolTable global;
		global.name = "Global";
		SymbolTableRecord record;
		record.name = "Test";
		record.kind = kind_class;
		// Third param is not specified
		SymbolTableRecord* ptr = global.addRecord(record.name, record, &global, true);
		Assert::IsNotNull(ptr->scope.get());
	};


	TEST_METHOD(SymbolTable_nameTest) {
		SymbolTable global;
		global.name = "Global";
		global.label = "Global";
		SymbolTableRecord record;
		record.name = "Test";
		record.kind = kind_class;
		// Third param is specified as the paret
		SymbolTableRecord* ptr = global.addRecord(record.name, record, &global, true);
		Assert::IsTrue(global.name == "Global");
		Assert::IsTrue(global.label == "Global");

		Assert::IsTrue(ptr->scope->name == "Test");
		Assert::IsTrue(ptr->scope->label == "Global.Test");
	};

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

	TEST_METHOD(SymbolTable_sourceFilesTest) {
		// TODO: if this test fails, that means the grammar.txt file is out of date
		Compiler c(GetDirectoryName(TEST_CASE_DIRECTORY) + "grammar.txt", "prog", false);
		c.setSourceFile(GetDirectoryName(TEST_CASE_DIRECTORY) + "SymbolTableTestSource.txt");
		c.compile();

		// Found Tables
		SymbolTable actual_globalTable = c.getSymbolTable();
		SymbolTable actual_MyClass1Table = *actual_globalTable.find("MyClass1").first->scope.get();
		SymbolTable actual_MyClass2Table = *actual_globalTable.find("MyClass2").first->scope.get();
		SymbolTable actual_f1Table = *actual_globalTable.find("f1").first->scope.get();
		SymbolTable actual_f2Table = *actual_globalTable.find("f2").first->scope.get();
		SymbolTable actual_programTable = *actual_globalTable.find("program").first->scope.get();
		SymbolTable actual_MyClass1_mc1f1 = *actual_MyClass1Table.find("mc1f1").first->scope.get();
		SymbolTable actual_MyClass1_f2 = *actual_MyClass1Table.find("f2").first->scope.get();


		Assert::IsNotNull(&actual_globalTable);
		Assert::IsNotNull(&actual_MyClass1Table);
		Assert::IsNotNull(&actual_MyClass2Table);
		Assert::IsNotNull(&actual_f1Table);
		Assert::IsNotNull(&actual_f2Table);
		Assert::IsNotNull(&actual_programTable);
		Assert::IsNotNull(&actual_MyClass1_mc1f1);
		Assert::IsNotNull(&actual_MyClass1_f2);



		// Build the entire table in order to compare it

		SymbolTable expected_globalTable;
		expected_globalTable.name = "Global";
		expected_globalTable.label = "Global";
		SymbolTableRecord expected_globalTable__f1;
		expected_globalTable__f1.name = "f1";
		expected_globalTable__f1.kind = kind_function;
		std::vector<std::pair<TypeStruct, std::string>> expected_globalTable__f1_parameters;
		expected_globalTable__f1_parameters.push_back({ { type_int, struct_array, "", {2,2} }, "fp1" });
		expected_globalTable__f1_parameters.push_back({ { type_float, struct_simple, "",{  } }, "fp2" });
		expected_globalTable__f1.functionData = { { type_float, struct_simple, "",{} },expected_globalTable__f1_parameters };
		SymbolTableRecord expected_globalTable__f2;
		expected_globalTable__f2.name = "f2";
		expected_globalTable__f2.kind = kind_function;
		expected_globalTable__f2.functionData = { { type_int, struct_simple, "",{} },{} };
		SymbolTableRecord expected_globalTable__MyClass1;
		expected_globalTable__MyClass1.name = "MyClass1";
		expected_globalTable__MyClass1.kind = kind_class;
		SymbolTableRecord expected_globalTable__MyClass2;
		expected_globalTable__MyClass2.name = "MyClass2";
		expected_globalTable__MyClass2.kind = kind_class;
		SymbolTableRecord expected_globalTable__program;
		expected_globalTable__program.name = "program";
		expected_globalTable__program.kind = kind_function;
		expected_globalTable__program.functionData = { { type_none, struct_simple, "", {} }, {} };

		SymbolTableRecord* expected_globalTable__f1scope = expected_globalTable.addRecord(expected_globalTable__f1.name, expected_globalTable__f1, &expected_globalTable, true);
		SymbolTableRecord* expected_globalTable__f2scope = expected_globalTable.addRecord(expected_globalTable__f2.name, expected_globalTable__f2, &expected_globalTable, true);
		SymbolTableRecord* expected_globalTable_MyClass1Scope = expected_globalTable.addRecord(expected_globalTable__MyClass1.name, expected_globalTable__MyClass1, &expected_globalTable, true);
		SymbolTableRecord* expected_globalTable_MyClass2Scope = expected_globalTable.addRecord(expected_globalTable__MyClass2.name, expected_globalTable__MyClass2, &expected_globalTable, true);
		SymbolTableRecord* expected_globalTable_programScope = expected_globalTable.addRecord(expected_globalTable__program.name, expected_globalTable__program, &expected_globalTable, true);


		SymbolTable expected_MyClass1Table = *expected_globalTable_MyClass1Scope->scope.get();
		SymbolTableRecord expected_MyClass1Table__mc1v1;
		expected_MyClass1Table__mc1v1.name = "mc1v1";
		expected_MyClass1Table__mc1v1.kind = kind_variable;
		expected_MyClass1Table__mc1v1.typeStructure = { type_int, struct_array, "",{ 2,4 } };
		SymbolTableRecord expected_MyClass1Table__mc1v2;
		expected_MyClass1Table__mc1v2.name = "mc1v2";
		expected_MyClass1Table__mc1v2.kind = kind_variable;
		expected_MyClass1Table__mc1v2.typeStructure = { type_float, struct_simple, "",{} };
		SymbolTableRecord expected_MyClass1Table__mc1v3;
		expected_MyClass1Table__mc1v3.name = "mc1v3";
		expected_MyClass1Table__mc1v3.kind = kind_variable;
		expected_MyClass1Table__mc1v3.typeStructure = { type_class, struct_array, "MyClass2",{ 3 } };
		SymbolTableRecord expected_MyClass1Table__mc1f1;
		expected_MyClass1Table__mc1f1.name = "mc1f1";
		expected_MyClass1Table__mc1f1.kind = kind_function;
		std::vector<std::pair<TypeStruct, std::string>> expected_MyClass1Table__mc1f1_parameters;
		expected_MyClass1Table__mc1f1_parameters.push_back({ {type_int, struct_simple, "", {}}, "p1" });
		expected_MyClass1Table__mc1f1_parameters.push_back({ { type_class, struct_array, "MyClass2",{3} }, "p2" });
		expected_MyClass1Table__mc1f1.functionData = { {type_int, struct_simple, "", {}}, expected_MyClass1Table__mc1f1_parameters };
		SymbolTableRecord expected_MyClass1Table__f2;
		expected_MyClass1Table__f2.name = "f2";
		expected_MyClass1Table__f2.kind = kind_function;
		std::vector<std::pair<TypeStruct, std::string>> expected_MyClass1Table__f2_parameters;
		expected_MyClass1Table__f2_parameters.push_back({ { type_class, struct_array, "MyClass1",{ 3 } }, "f2p1" });
		expected_MyClass1Table__f2.functionData = { { type_int, struct_simple, "",{} }, expected_MyClass1Table__f2_parameters };
		expected_MyClass1Table.addRecord(expected_MyClass1Table__mc1v1.name, expected_MyClass1Table__mc1v1, NULL, false), 
		expected_MyClass1Table.addRecord(expected_MyClass1Table__mc1v2.name, expected_MyClass1Table__mc1v2, NULL, false);
		expected_MyClass1Table.addRecord(expected_MyClass1Table__mc1v3.name, expected_MyClass1Table__mc1v3, NULL, false);
		SymbolTableRecord* expected_MyClass1Table__mcf1scope = expected_MyClass1Table.addRecord(expected_MyClass1Table__mc1f1.name, expected_MyClass1Table__mc1f1, &expected_MyClass1Table, true);
		SymbolTableRecord* expected_MyClass1Table__f2scope = expected_MyClass1Table.addRecord(expected_MyClass1Table__f2.name, expected_MyClass1Table__f2, &expected_MyClass1Table, true);




		SymbolTable expected_MyClass2Table = *expected_globalTable_MyClass2Scope->scope.get();
		SymbolTableRecord expected_MyClass2Table__mc1v1;
		expected_MyClass2Table__mc1v1.name = "mc1v1";
		expected_MyClass2Table__mc1v1.kind = kind_variable;
		expected_MyClass2Table__mc1v1.typeStructure = { type_int, struct_array, "", {2,4} };
		SymbolTableRecord expected_MyClass2Table__fp1;
		expected_MyClass2Table__fp1.name = "fp1";
		expected_MyClass2Table__fp1.kind = kind_variable;
		expected_MyClass2Table__fp1.typeStructure = { type_float, struct_simple, "",{} };
		SymbolTableRecord expected_MyClass2Table__m2;
		expected_MyClass2Table__m2.name = "m2";
		expected_MyClass2Table__m2.kind = kind_variable;
		expected_MyClass2Table__m2.typeStructure = { type_class, struct_array, "MyClass2",{3} };
		expected_MyClass2Table.addRecord(expected_MyClass2Table__mc1v1.name, expected_MyClass2Table__mc1v1, NULL, false);
		expected_MyClass2Table.addRecord(expected_MyClass2Table__fp1.name, expected_MyClass2Table__fp1, NULL, false);
		expected_MyClass2Table.addRecord(expected_MyClass2Table__m2.name, expected_MyClass2Table__m2, NULL, false);


		SymbolTable expected_f1Table = *expected_globalTable__f1scope->scope.get();
		SymbolTableRecord expected_f1Table__fp1;
		expected_f1Table__fp1.name = "fp1";
		expected_f1Table__fp1.kind = kind_parameter;
		expected_f1Table__fp1.typeStructure = { type_int, struct_array, "", {2,2} };
		SymbolTableRecord expected_f1Table__fp2;
		expected_f1Table__fp2.name = "fp2";
		expected_f1Table__fp2.kind = kind_parameter;
		expected_f1Table__fp2.typeStructure = { type_float , struct_simple, "",{ } };
		SymbolTableRecord expected_f1Table__fv1;
		expected_f1Table__fv1.name = "fv1";
		expected_f1Table__fv1.kind = kind_variable;
		expected_f1Table__fv1.typeStructure = { type_class, struct_array, "MyClass1",{ 3} };
		SymbolTableRecord expected_f1Table__fv2;
		expected_f1Table__fv2.name = "fv2";
		expected_f1Table__fv2.kind = kind_variable;
		expected_f1Table__fv2.typeStructure = { type_int, struct_simple, "",{} };
		expected_f1Table.addRecord(expected_f1Table__fp1.name, expected_f1Table__fp1, NULL, false);
		expected_f1Table.addRecord(expected_f1Table__fp2.name, expected_f1Table__fp2, NULL, false);
		expected_f1Table.addRecord(expected_f1Table__fv1.name, expected_f1Table__fv1, NULL, false);
		expected_f1Table.addRecord(expected_f1Table__fv2.name, expected_f1Table__fv2, NULL, false);


		// Empty table
		SymbolTable expected_f2Table = *expected_globalTable__f2scope->scope.get();
		

		SymbolTable expected_programTable = *expected_globalTable_programScope->scope.get();
		SymbolTableRecord expected_programTable__m1;
		expected_programTable__m1.name = "m1";
		expected_programTable__m1.kind = kind_variable;
		expected_programTable__m1.typeStructure = { type_int, struct_simple, "",{} };
		SymbolTableRecord expected_programTable__m2;
		expected_programTable__m2.name = "m2";
		expected_programTable__m2.kind = kind_variable;
		expected_programTable__m2.typeStructure = { type_float, struct_array, "",{3,2} };
		SymbolTableRecord expected_programTable__m3;
		expected_programTable__m3.name = "m3";
		expected_programTable__m3.kind = kind_variable;
		expected_programTable__m3.typeStructure = { type_class, struct_array, "MyClass2",{ 2 } };
		expected_programTable.addRecord(expected_programTable__m1.name, expected_programTable__m1, NULL, false);
		expected_programTable.addRecord(expected_programTable__m2.name, expected_programTable__m2, NULL, false);
		expected_programTable.addRecord(expected_programTable__m3.name, expected_programTable__m3, NULL, false);



		SymbolTable expected_MyClass1_mc1f1 = *expected_MyClass1Table__mcf1scope->scope.get();
		SymbolTableRecord expected_MyClass1_mc1f1__p1;
		expected_MyClass1_mc1f1__p1.name = "p1";
		expected_MyClass1_mc1f1__p1.kind = kind_parameter;
		expected_MyClass1_mc1f1__p1.typeStructure = { type_int, struct_simple, "", {} };
		SymbolTableRecord expected_MyClass1_mc1f1__p2;
		expected_MyClass1_mc1f1__p2.name = "p2";
		expected_MyClass1_mc1f1__p2.kind = kind_parameter;
		expected_MyClass1_mc1f1__p2.typeStructure = { type_class, struct_array, "MyClass2",{3} };
		SymbolTableRecord expected_MyClass1_mc1f1__fv1;
		expected_MyClass1_mc1f1__fv1.name = "fv1";
		expected_MyClass1_mc1f1__fv1.kind = kind_variable;
		expected_MyClass1_mc1f1__fv1.typeStructure = { type_class, struct_array, "MyClass2",{ 3 } };
		expected_MyClass1_mc1f1.addRecord(expected_MyClass1_mc1f1__p1.name, expected_MyClass1_mc1f1__p1, NULL, false);
		expected_MyClass1_mc1f1.addRecord(expected_MyClass1_mc1f1__p2.name, expected_MyClass1_mc1f1__p2, NULL, false);
		expected_MyClass1_mc1f1.addRecord(expected_MyClass1_mc1f1__fv1.name, expected_MyClass1_mc1f1__fv1, NULL, false);

		


		SymbolTable expected_MyClass1_f2 = *expected_MyClass1Table__f2scope->scope.get();
		SymbolTableRecord expected_MyClass1_f2__f2p1;
		expected_MyClass1_f2__f2p1.name = "f2p1";
		expected_MyClass1_f2__f2p1.kind = kind_parameter;
		expected_MyClass1_f2__f2p1.typeStructure = { type_class, struct_array, "MyClass1", {3} };
		SymbolTableRecord expected_MyClass1_f2__mc1v1;
		expected_MyClass1_f2__mc1v1.name = "mc1v1";
		expected_MyClass1_f2__mc1v1.kind = kind_variable;
		expected_MyClass1_f2__mc1v1.typeStructure = { type_int, struct_simple, "", {} };;
		expected_MyClass1_f2.addRecord(expected_MyClass1_f2__f2p1.name, expected_MyClass1_f2__f2p1, NULL, false);
		expected_MyClass1_f2.addRecord(expected_MyClass1_f2__mc1v1.name, expected_MyClass1_f2__mc1v1, NULL, false);



		Assert::IsTrue(actual_globalTable == expected_globalTable);
		Assert::IsTrue(actual_MyClass1Table == expected_MyClass1Table);
		Assert::IsTrue(actual_MyClass2Table == expected_MyClass2Table);
		Assert::IsTrue(actual_f1Table == expected_f1Table);
		Assert::IsTrue(actual_f2Table == expected_f2Table);
		Assert::IsTrue(actual_programTable == expected_programTable);
		Assert::IsTrue(actual_MyClass1_mc1f1 == expected_MyClass1_mc1f1);
		Assert::IsTrue(actual_MyClass1_f2 == expected_MyClass1_f2);
	};

	};
}
