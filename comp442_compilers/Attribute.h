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
	attr_var
};

struct Attributes {
	AttributeType type;
	Expression expr;
	StatementData statementData;
	Variable var;
};
#endif
