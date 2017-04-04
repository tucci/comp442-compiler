#ifndef STAT_BLOCK_H
#define STAT_BLOCK_H


struct StatementBlock {
	std::vector<std::shared_ptr<Statement>> statements;
	std::string _toMoonCode();
	

};

inline std::string StatementBlock::_toMoonCode() {
	std::string statBlock;
	for (std::vector<std::shared_ptr<Statement>>::value_type statement : statements) {
		statBlock.append(statement->_toMoonCode());
		
	}
	return statBlock;
}


#endif

