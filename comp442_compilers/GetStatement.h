#ifndef GET_STATEMENT_H
#define GET_STATEMENT_H

class GetStatement : public Statement {
public:
	GetStatement();
	~GetStatement();
	Variable var;


	std::string _toMoonCode() override;
};
#endif

