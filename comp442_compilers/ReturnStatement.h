#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
class ReturnStatement : public Statement {
public:
	ReturnStatement();
	~ReturnStatement();
	TypeStruct functionReturnType;
	Expression returnExpression;

	std::string _toMoonCode() override;
};

#endif
