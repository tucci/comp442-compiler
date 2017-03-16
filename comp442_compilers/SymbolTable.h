#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


// TODO: figure out undefined id : variable/class/function
// TODO: figure out member: data member, method, deeply nested
// TODO: figure out how to handle forward/circular references/two phase
// TODO: figure out how to handle multiple defined var names
// TODO: figure out how to handle recursion
// TODO: type check return statement
// TODO: type check assignment statement
// TODO: type check function calls params have right amount and types
class SymbolTable {
public:
	// Create a symbol table with no parent
	SymbolTable();
	~SymbolTable();
	// Finds the given identifer into the symbol table
	// returns a pair, where the second value is if the value is found, and the first value is a pointer to the record
	std::pair<SymbolTableRecord*, bool> find(const std::string& identifier);
	// Adds the given record with the identifer to the symbol table
	SymbolTableRecord* addRecord(const std::string& identifier, SymbolTableRecord record, SymbolTable* parent=NULL);
	// Remove the record if it the identifer is in the symbol table. Returns true if the deletion was successful, false otherwise
	bool removeRecord(const std::string& identifier);

	// Outputs the contents of the symbol table and any child symbol table
	// Does a breadth first traversal
	std::string toString();

	// The link to the parent table of this symbol table
	SymbolTable* parent;
	
	// A name to be added to the table for easier outputing
	std::string tableTagName;
private:
	// TODO function overloading may be allowed, storing the name as a key might not work
	std::unordered_map<std::string, SymbolTableRecord> table;
};

#endif // !SYMBOL_TABLE_H

