#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(SymbolTableTest) {
public:

	TEST_METHOD(SymbolTable_globalTest) {
		SymbolTable global;
		Assert::IsNull(global.parent);
	};

	TEST_METHOD(SymbolTable_childTableTest) {
		SymbolTable global;
		SymbolTable child(&global);
		Assert::IsNotNull(child.parent);
	};

	TEST_METHOD(SymbolTable_addRecordTest) {
		SymbolTable global;
		SymbolTableRecord record;
		record.properlyDeclared = true;
		record.name = "testVar";
		record.kind = SymbolKind::kind_variable;
		record.typeStructure = { SymbolType::type_int, SymbolStructure::struct_simple, "", {} };
		record.functionData = {};// none
		record.link = NULL; // no Link
		record.address = 0; // no address now
		SymbolTableRecord* ptr = global.addRecord(record.name, record);
		Assert::IsNotNull(ptr);
	};

	TEST_METHOD(SymbolTable_addRecordWithLinkTest) {
		SymbolTable global;
		SymbolTable functionTable(&global);

		SymbolTableRecord record;
		record.properlyDeclared = true;
		record.name = "testFunc";
		record.kind = SymbolKind::kind_function;
		record.typeStructure = {};
		record.functionData = { {SymbolType::type_int, SymbolStructure::struct_simple, "", {}}, {} };
		record.link = &functionTable; // no Link
		record.address = 0; // no address now
		SymbolTableRecord* ptr = global.addRecord(record.name, record);
		Assert::IsNotNull(ptr);
	};


	};
}