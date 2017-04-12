#ifndef STAT_BLOCK_H
#define STAT_BLOCK_H


// A statment block is just a list of statements
struct StatementBlock {
	// the list of statements in this block
	std::vector<std::shared_ptr<Statement>> statements;
	// the method that outputs all the statments into a string
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

