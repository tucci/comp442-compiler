#include "stdafx.h"
#include "SymbolTable.h"


SymbolTable::SymbolTable() {
	parent = NULL;
}

SymbolTable::~SymbolTable() {

}

std::pair<SymbolTableRecord*, bool> SymbolTable::find(const std::string& identifier) {
	std::unordered_map<std::string, SymbolTableRecord>::iterator found = table.find(identifier);

	if (found == table.end()) {
		// The identifer is not found in this table
		return std::make_pair<SymbolTableRecord*, bool>(NULL, false);
	}
	// Identifer is found, and return a pointer to it
	return std::make_pair<SymbolTableRecord*, bool>(&found->second, true);
}

std::pair<SymbolTableRecord*, bool> SymbolTable::findInParents(const std::string& identifier) {
	std::unordered_map<std::string, SymbolTableRecord>::iterator found = table.find(identifier);

	if (found == table.end()) {
		if (parent != NULL) {
			return parent->findInParents(identifier);
		}
		return std::make_pair<SymbolTableRecord*, bool>(NULL, false);
	}
	// Identifer is found, and return a pointer to it
	return std::make_pair<SymbolTableRecord*, bool>(&found->second, true);
}

SymbolTableRecord* SymbolTable::addRecord(const std::string& identifier, SymbolTableRecord record, SymbolTable* parent, bool needsLink) {
	// Newly added duplicate names are dropped right away
	if (find(identifier).second) {
		// If this record is already added, drop
		return NULL;
	}
	std::unordered_map<std::string, SymbolTableRecord>::_Pairib emplacement = table.emplace(identifier, record);
	SymbolTableRecord* addedRecord = &emplacement.first->second;
	if (parent != NULL) {
		std::string label = parent->resolvedName + "_" + identifier;;
		addedRecord->label = label;
		if (needsLink) {
			addedRecord->scope = std::shared_ptr<SymbolTable>(new SymbolTable());
			addedRecord->scope->parent = parent;
			addedRecord->scope->resolvedName = label;
			addedRecord->scope->name = identifier;
		}
	} else {
		addedRecord->label = identifier;
	}
	// Return a pointer to this table
	return addedRecord;
}





std::string SymbolTable::toString() {
	std::queue<SymbolTable*> queue;
	std::string output;
	queue.push(this);

	while (queue.size() > 0) {
		SymbolTable* n = queue.front();
		queue.pop();
		output += "\nTag: " + n->resolvedName + "\n";
		for (auto record : n->table) {
			SymbolTable* scope = record.second.scope.get();
			output += record.first + " -> " + record.second.toString();
			if (scope != NULL) {
				queue.push((scope));
			}
		}
	}
	return output;
}

int SymbolTable::_sizeOf(SymbolTable* globalTable, TypeStruct idType) {
	int typeSize = 0;
	if (idType.type == SymbolType::type_int || idType.type == SymbolType::type_float) {
		// Integers and floats are stored as 32bits/4 bytes
		typeSize = WORD_SIZE_IN_BYTES;
	} else if (idType.type == SymbolType::type_class) {
		SymbolTable* classTable = globalTable->find(idType.className).first->scope.get();
		// the class needs to be computed
		typeSize = classTable->sizeOfTable(globalTable);
	}

	if (idType.structure == SymbolStructure::struct_simple) {
		return typeSize;
	}

	else if (idType.structure == SymbolStructure::struct_array) {
		int flattenedDimSize = 1;
		for (int dimSize : idType.dimensions) {
			flattenedDimSize *= dimSize;
		}
		return  flattenedDimSize * typeSize;
	} else {
		throw std::exception("The type could not be determined. The type is not a simple type or array");
	}
};


int SymbolTable::sizeOfTable(SymbolTable* globalTable) {
	int size = 0;
	for (std::unordered_map<std::basic_string<char>, SymbolTableRecord>::value_type record : table) {
		TypeStruct type = record.second.typeStructure;
		if (record.second.kind == SymbolKind::kind_variable) {
			size += _sizeOf(globalTable, type);
		}
	
	}
	return size;
}



bool operator==(const SymbolTable& lhs, const SymbolTable& rhs) {
	if (lhs.table.size() != rhs.table.size()) return false;
	if (lhs.name != rhs.name) return false;
	if (lhs.resolvedName!= rhs.resolvedName) return false;
	for (const std::unordered_map<std::string, SymbolTableRecord>::value_type& record : lhs.table) {
		std::string lhsIdentifier = record.first;
		std::unordered_map<std::string, SymbolTableRecord>::const_iterator foundInRhs = rhs.table.find(lhsIdentifier);
		// It didnt find it
		if (foundInRhs == rhs.table.end()) {
			return false;
		} else {
			if (!(record.second == foundInRhs->second)) {
				return false;
			}
		}

		
	}
	return true;
}
