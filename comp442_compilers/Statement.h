#ifndef STATEMENT_H
#define STATEMENT_H

struct TypeStruct;

enum StatementType {
	assignStat,
	forStat,
	ifelseStat,
	getStat,
	putStat,
	returnStat,
};

struct AssignStatement {
	Variable var;
	Expression expression;
};

struct ForStatement {};
struct IfElseStatement {};

struct GetStatement {
	Variable var;
};
struct PutStatement{};
struct ReturnStatement {
	TypeStruct functionReturnType;
	Expression returnExpression;
};


struct StatementData {
	AssignStatement assignStatement;
	ForStatement forStatement;
	IfElseStatement ifElseStatement;
	GetStatement getStatement;
	PutStatement putStatement;
	ReturnStatement returnStatement;
};


class Statement {
public:
	Statement();
	~Statement();
	StatementType statType;
	StatementData statData;
};

#endif
