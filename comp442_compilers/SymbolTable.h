#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


// TODO: output symbol table
// TODO: figure out undefined ids/vars
// TODO: figure out how to handle multiple defined var names/ 
// TODO: figure out how to handle recursion
// TODO: create for int i, semantic action
// TODO: make sure array dimensions are done properly
// TODO: type check return statement
// TODO: type check assignment statement
// TODO: function calls should have right amount
// TODO: when adding a link we'll need to set up the data in this record and add it to the nested table. ex func data, class data
class SymbolTable {
public:
	// Create a symbol table with no parent
	SymbolTable();
	// Create a symbol table with the given parent symbol table
	SymbolTable(SymbolTable* parent);
	~SymbolTable();
	// Finds the given identifer into the symbol table
	// returns a pair, where the second value is if the value is found, and the first value is a pointer to the record
	std::pair<SymbolTableRecord*, bool> find(const std::string& identifier);
	// Adds the given record with the identifer to the symbol table
	SymbolTableRecord* addRecord(const std::string& identifier, SymbolTableRecord record);
	// Remove the record if it the identifer is in the symbol table. Returns true if the deletion was successful, false otherwise
	bool removeRecord(const std::string& identifier);

	// Outputs the contents of the symbol table and any child symbol table
	std::string toString();

	// The link to the parent table of this symbol table
	SymbolTable* parent;
	
private:
	// TODO function overloading may be allowed, storing the name as a key might not work
	std::unordered_map<std::string, SymbolTableRecord> table;
};

#endif // !SYMBOL_TABLE_H

