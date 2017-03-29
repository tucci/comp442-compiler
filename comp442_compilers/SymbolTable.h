#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


// TODO: figure out how to handle recursion
// TODO: type check return statement
// TODO: type check function calls params have right amount and types
class SymbolTable {
public:
	friend class SemanticActions;
	// Create a symbol table with no parent
	SymbolTable();
	~SymbolTable();
	// Finds the given identifer into the symbol table
	// returns a pair, where the second value is if the value is found, and the first value is a pointer to the record
	std::pair<SymbolTableRecord*, bool> find(const std::string& identifier);
	// Finds the given identifer into the symbol table or parent tables
	// returns a pair, where the second value is if the value is found, and the first value is a pointer to the record
	std::pair<SymbolTableRecord*, bool> findInParents(const std::string& identifier);
	// Adds the given record with the identifer to the symbol table
	SymbolTableRecord* addRecord(const std::string& identifier, SymbolTableRecord record, SymbolTable* parent=NULL);
	friend bool operator==(const SymbolTable& lhs, const SymbolTable& rhs);

	// Outputs the contents of the symbol table and any child symbol table
	// Does a breadth first traversal
	std::string toString();

	// The link to the parent table of this symbol table
	SymbolTable* parent;
	
	// A name to be added to the table for easier outputing
	std::string resolvedName;
	// The base name for this table
	std::string name;
private:
	std::unordered_map<std::string, SymbolTableRecord> table;
	// Used during circular table
	bool marked;

	
};

#endif // !SYMBOL_TABLE_H

