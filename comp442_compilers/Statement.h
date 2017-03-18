#ifndef STATEMENT_H
#define STATEMENT_H

enum StatmentType {
	assignStat,
	forStat,
	ifelseStat,
	getStat,
	putStat,
	returnStat,
};

struct AssignStatment {
	Variable var;
	Expression expression;
};

struct ForStatement {};
struct IfElseStatement {};
struct GetStatement{};
struct PutStatment{};
struct ReturnStatment{};


struct StatementData {
	AssignStatment assignStatement;
	ForStatement forStatement;
	IfElseStatement ifElseStatement;
	GetStatement getStatement;
	PutStatment putStatement;
	ReturnStatment returnStatement;
};


class Statement {
public:
	Statement();
	~Statement();
	StatmentType statType;
	StatementData statData;
};

#endif
