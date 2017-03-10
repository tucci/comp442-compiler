#ifndef SYMBOL_TABLE_DATA_H
#define SYMBOL_TABLE_DATA_H


// List of symbols kinds
enum SymbolKind {
	kind_variable,
	kind_parameter,
	kind_function,
	kind_class
};

// List of symbol types that can be in the lanaguage
enum SymbolType {
	type_int,
	type_float,
	type_class,
};

// List of structures that can be in the language
enum SymbolStructure {
	struct_simple,
	struct_array
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

static std::string typeToString(SymbolType type) {
	switch (type) {
	case type_int:		return "type_int"; break;
	case type_float:	return "type_float"; break;
	case type_class:	return "type_class"; break;
	default:			return "type_error"; break;
	}
}

static std::string structureToString(SymbolStructure structure) {
	switch (structure) {
	case struct_simple:	return "struct_simple"; break;
	case struct_array:	return "struct_array"; break;
	default:			return "struct_error"; break;
	}
}

// A simple wrapper around type/struct
struct TypeStruct {
	SymbolType type;
	SymbolStructure structure;
	std::string className; // Used if there is a class name
	std::vector<int> dimensions; // Used if there is array data

	std::string toString() {
		std::string s = typeToString(type);
		if (type == SymbolType::type_class) {
			s += " " + className;
		}
		if (structure == SymbolStructure::struct_array) {
			for (int& dim : dimensions) {
				s += "[" + std::to_string(dim) + "]";
			}
		}
		s += ":" + structureToString(structure);
		return s;
	}
};

// Holds all the data for a function declaration
struct FunctionData {
	// The return values type and struct
	// This is allowing for returning of arrays
	TypeStruct returnType;

	// holds pairs of types and the array data if needed
	std::vector<TypeStruct> parameters;

	std::string toString() {
		std::string s = "return type=" + returnType.toString();
		s += ", params={";
		if (parameters.size() == 0) {
			s += "nil";
		} else {
			for (TypeStruct& ts : parameters) {
				s += ts.toString() + ", ";
			}
		}
		s += "}";
		return s;
	};
};



// Forward declration
class SymbolTable;


// A simple record that will go into the symbol tables
struct SymbolTableRecord {
	// Whether this identifier has been declared properly
	bool properlyDeclared;
	// The name of this symbol
	std::string name;
	// The kind of symbol this is. var/param/function/class
	SymbolKind kind;
	// The symbol's type and structure. int,float,class -> simple/array and any array data
	TypeStruct typeStructure;
	// used when this type is a function type
	FunctionData functionData;
	// The link to the child symbol table
	SymbolTable* link;
	// The address of the element in memory
	int address;

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

};

#endif // !SYMBOL_TABLE_DATA_H