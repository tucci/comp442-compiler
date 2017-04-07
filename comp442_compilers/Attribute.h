#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H



enum StatementType {
	assignStat,
	forStat,
	ifelseStat,
	getStat,
	putStat,
	returnStat,
};

struct StatementData {
	StatementType statType;
	AssignStatement assignStatement;
	ForStatement forStatement;
	IfElseStatement ifelseStatement;
	GetStatement getStatement;
	PutStatement putStatement;
	ReturnStatement returnStatement;
};





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

struct Attributes {
	AttributeType type;
	Expression expr;
	StatementData statementData;
	Variable var;
	FunctionDeclStatementList funcDef;

};

#endif
