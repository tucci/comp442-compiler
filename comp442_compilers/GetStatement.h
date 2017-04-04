#ifndef GET_STATEMENT_H
#define GET_STATEMENT_H
#include "Register.h"

class GetStatement : public Statement {
public:
	GetStatement();
	~GetStatement();
	Variable var;


	std::string _toMoonCode() override;
};
#endif

