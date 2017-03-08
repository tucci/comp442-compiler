#include "stdafx.h"


using namespace std;
int main() {
	//Compiler cp;
	//cp.setSourceFile("sourcefile.txt");
	//try {
	//	//  Hold try catch for special cases
	//	// inorder to free memory
	//	cp.analyzeSyntax();
	//} catch (std::exception e) {
	//
	//}

	/*SymbolTableRecord record = { "f1",
						SymbolKind::kind_variable,
						{ SymbolType::type_class, SymbolStructure::struct_array, "MyClass1" },
						{},
						{{1,2,3,4}},
						NULL };*/

	/*SymbolTableRecord record = { "f1",
		SymbolKind::kind_parameter,
		{ SymbolType::type_class, SymbolStructure::struct_array, "MyClass1", {1,2,3,4} },
		{},
		NULL };*/

	SymbolTableRecord record = { "f1",
		SymbolKind::kind_function,
		{ SymbolType::type_int, SymbolStructure::struct_simple },
		/*Function data*/
		{
			{ // list of params
				/*param 1*/	{SymbolType::type_int, SymbolStructure::struct_array, "",{2, 2}},
				/*param 2*/	{SymbolType::type_class, SymbolStructure::struct_array, "MyClass1", { 6 }},
				/*param 3*/	{SymbolType::type_class, SymbolStructure::struct_simple, "MyClass1",{ }}
			},
			// return type 
			{SymbolType::type_float, SymbolStructure::struct_simple, "", {}}
		},
		NULL };

	//SymbolTableRecord record = { "f1",
	//	SymbolKind::kind_function,
	//	{ SymbolType::type_int, SymbolStructure::struct_simple, "", {} },
	//	{ // function data
	//		{/* list of params */},
	//		// return type 
	//		 {SymbolType::type_float, SymbolStructure::struct_simple, "", {}},
	//	},
	//	NULL };

	std::cout << record.toString();


	return 0;
}






