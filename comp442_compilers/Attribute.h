#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H


// The type of statements we have in our program
enum StatementType {
	assignStat,
	forStat,
	ifelseStat,
	getStat,
	putStat,
	returnStat,
};


// A grouping of statements
// Usually only needs one statment at a time
struct StatementData {
	StatementType statType;
	AssignStatement assignStatement;
	ForStatement forStatement;
	IfElseStatement ifelseStatement;
	GetStatement getStatement;
	PutStatement putStatement;
	ReturnStatement returnStatement;
};




// The type of attributes
enum AttributeType {
	attr_expr,
	attr_statement,
	attr_var,
	attr_funcDef
};

// Holds all the data about a functions declrations
// and its inner statements
struct FunctionDeclStatementList {
	SymbolTableRecord* functionRecord;
	std::vector<std::shared_ptr<Statement>> statements;
};

// All the neccessary attributes needed to build up data while performing semantic actions
struct Attributes {
	// the type of attribute this is
	AttributeType type;
	Expression expr;
	StatementData statementData;
	Variable var;
	FunctionDeclStatementList funcDef;
};

#endif
