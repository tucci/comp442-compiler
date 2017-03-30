#ifndef SYMBOL_TABLE_DATA_H
#define SYMBOL_TABLE_DATA_H
#include "Statement.h"
#include "TypeData.h"
#include <memory>
#include "Attribute.h"


// List of symbols kinds
enum SymbolKind {
	kind_variable,
	kind_parameter,
	kind_function,
	kind_class,
	kind_error
};



static std::string kindToString(SymbolKind kind) {
	switch (kind) {
	case kind_variable:		return "kind_variable"; break;
	case kind_parameter:	return "kind_parameter"; break;
	case kind_function:		return "kind_function"; break;
	case kind_class:		return "kind_class"; break;
	default:				return "kind_error"; break;
	}
}



// Forward declration
class SymbolTable;


// A simple record that will go into the symbol tables
class SymbolTableRecord {
public:
	// TODO: implment Whether this identifier has been declared properly
	bool properlyDeclared;
	// The name of this symbol
	std::string name;
	// The kind of symbol this is. var/param/function/class
	SymbolKind kind;
	// The symbol's type and structure. int,float,class -> simple/array and any array data
	TypeStruct typeStructure;
	// used when this type is a function type
	FunctionDeclarationData functionData;
	// The link to the child symbol table
	std::shared_ptr<SymbolTable> scope;
	// On what line/location of the source code is this identifer defined
	int definedLocation;
	// The address of the element in memory
	int address;


	// The attributes of this semantic record
	// Used to build up expressions/statements and other values while parsing
	Attributes attr;

	std::string toString() {

		std::string s = "Name=" + name + " -> Kind=" + kindToString(kind);
		if (kind == SymbolKind::kind_variable || kind == SymbolKind::kind_parameter) {
			s += ": {Type: " + typeStructure.toString() + "}";
		}
		if (kind == SymbolKind::kind_function) {
			s += ": {" + functionData.toString() + "}";
		}
		return s + "\n";

	};

	friend bool operator==(const SymbolTableRecord& r1, const SymbolTableRecord& r2) {
		if (r1.name != r2.name) return false;
		if (r1.kind != r2.kind) return false;
		if (!(r1.typeStructure == r2.typeStructure)) return false;
		if (!(r1.functionData == r2.functionData)) return false;
		return true;
	}

};

#endif // !SYMBOL_TABLE_DATA_H