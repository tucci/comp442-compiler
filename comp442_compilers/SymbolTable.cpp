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

SymbolTableRecord* SymbolTable::addRecord(const std::string& identifier, SymbolTableRecord record, SymbolTable* parent) {
	// TODO: figure out if how we want to deal with records that are already in the table
	// TODO: remove the auto
	auto emplacement = table.emplace(identifier, record);
	SymbolTableRecord* addedRecord = &emplacement.first->second;
	if (parent != NULL) {
		addedRecord->scope = std::shared_ptr<SymbolTable>(new SymbolTable());
		addedRecord->scope->parent = parent;
		addedRecord->scope->tableTagName = parent->tableTagName + "." + identifier;
	}
	// Return a pointer to this table
	return addedRecord;
}

bool SymbolTable::removeRecord(const std::string& identifier) {
	int numErased = table.erase(identifier);
	if (numErased != 0) {
		return true;
	}
	return false;
	
}

std::string SymbolTable::toString() {
	std::string output;
	std::vector<SymbolTable*> scopes;
	output += "\nTag: " + tableTagName + "\n";
	for (auto record : table) {
		output += record.first + " -> " + record.second.toString();
		SymbolTable* scope = record.second.scope.get();
		if (scope != NULL) {
			scopes.push_back(scope);
		}
	}
	for (SymbolTable* scope : scopes) {
		output += scope->toString();
	}
	return output;
}


