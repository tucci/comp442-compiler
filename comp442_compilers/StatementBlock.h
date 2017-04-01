#ifndef STAT_BLOCK_H
#define STAT_BLOCK_H

struct StatementData;// forward declare

struct StatementBlock {
	std::vector<StatementData> statements;
};

#endif

