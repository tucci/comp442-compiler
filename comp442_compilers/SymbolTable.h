#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


class SymbolTable {
public:
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
	SymbolTableRecord* addRecord(const std::string& identifier, SymbolTableRecord record, SymbolTable* parent, bool needsLink);
	friend bool operator==(const SymbolTable& lhs, const SymbolTable& rhs);
	// Outputs the contents of the symbol table and any child symbol table
	// Does a breadth first traversal
	std::string toString();
	// The link to the parent table of this symbol table
	SymbolTable* parent;
	// Returns the inner symbol table
	const std::unordered_map<std::string, SymbolTableRecord> getTable();
	// A name to be added to the table for easier outputing
	std::string label;
	// The base name for this table
	std::string name;
	// Returns the size of this type in bytes if it is a class
	static int _sizeOf(SymbolTable* globalTable, TypeStruct idType);
	// Used during circular table checking
	bool marked;
private:
	std::unordered_map<std::string, SymbolTableRecord> table;
	// Returns the size of this table in bytes
	int sizeOfTable(SymbolTable* globalTable);
	
};

#endif // !SYMBOL_TABLE_H

