#ifndef TYPEDATA_H
#define TYPEDATA_H

#include <string>
#include <vector>

#define WORD_SIZE_IN_BYTES 4

// List of symbol types that can be in the lanaguage
enum SymbolType {
	type_none,
	type_mismatch,
	type_int,
	type_float,
	type_class,
};

// List of structures that can be in the language
enum SymbolStructure {
	struct_simple,
	struct_array,
	struct_error,
};

static std::string typeToString(SymbolType type) {
	switch (type) {
	case type_int:		return "type_int"; break;
	case type_float:	return "type_float"; break;
	case type_class:	return "type_class"; break;
	case type_none:		return "type_none"; break;
	case type_mismatch:	return "type_mismatch"; break;
	default:			return "type_none"; break;
	}
}

static SymbolType stringToType(const std::string& str) {
	if (str == Specification::REVERSE_TOKEN_MAP.at(TokenType::int_token)) {
		// this is a int type
		return SymbolType::type_int;
	} else if (str == Specification::REVERSE_TOKEN_MAP.at(TokenType::float_token)) {
		// this is a float type
		return SymbolType::type_float;
	} else {
		// This is a class
		return SymbolType::type_class;
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
	TypeStruct() : type(type_none), structure(struct_simple) {}
	TypeStruct(SymbolType stype, SymbolStructure sstructure) : type(stype), structure(sstructure) {}
	TypeStruct(SymbolType stype, SymbolStructure sstructure, std::string sclassName, std::vector<int> dim)
		: type(stype),
		structure(sstructure),
		className(sclassName),
		dimensions(dim) {

	}

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

	friend bool operator==(const TypeStruct& lhs, const TypeStruct& rhs) {
		if (lhs.type != rhs.type) return false;
		if (lhs.structure != rhs.structure) return false;
		if (lhs.className != rhs.className) return false;
		if (lhs.dimensions != rhs.dimensions) return false;
		return true;
	}
};

// Holds all the data for a function declaration
struct FunctionDeclarationData {
	// The return values type and struct
	// This is allowing for returning of arrays
	TypeStruct returnType;

	// holds pairs of types and the array data if needed
	// where typestruct is the type, and string is the name of the param
	std::vector<std::pair<TypeStruct, std::string>> parameters;

	std::string toString() {
		std::string s = "return type=" + returnType.toString();
		s += ", params={";
		if (parameters.size() == 0) {
			s += "nil";
		} else {
			for (std::pair<TypeStruct, std::string>& param : parameters) {
				s += param.first.toString() + " " + param.second + ", ";
			}
		}
		s += "}";
		return s;
	};

	friend bool operator==(const FunctionDeclarationData& lhs, const FunctionDeclarationData& rhs) {
		if (!(lhs.returnType == rhs.returnType)) return false;
		if (lhs.parameters != rhs.parameters) return false;
		return true;
	}

};


#endif