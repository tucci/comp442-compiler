#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

class ForStatement : public Statement {
public:
	ForStatement();
	~ForStatement();

	StatementBlock statements;
	std::string _toMoonCode() override;
};

#endif
