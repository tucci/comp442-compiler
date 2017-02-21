#ifndef PARSER_H
#define PARSER_H


class Parser {
public:
	// Create a parser from a given lexer and grammar
	Parser(const Lexer& lexer, const Grammar& grammar);
	~Parser();
	// Parse method
	bool parse();
	
private:
	// Our lexer
	Lexer lexer;
	// The grammar our parser is trying to validate
	Grammar grammar;
	// The lookahead token for our parser to use
	Token lookAheadToken;
	// Data structure to hold first set
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> firstSet;
	// Data structure to hold follow set
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> followSet;
	// Data structe to hold the parse table
	std::unordered_map <NonTerminal, std::unordered_map <Terminal, Production, SymbolHasher, SymbolEqual>, SymbolHasher, SymbolEqual> parseTable;
	// Data structure to hold our parse stack
	std::stack<Symbol> parseStack;
	// Builds the parse table from the grammar
	void buildParseTable();
	// Handles the errors
	void skipErrors();
	// Pushes the rhs of this production in inverse order
	void inverseRHSMultiplePush(const Production& rhs);

	bool inFirst(const Terminal& terminal, const NonTerminal& nonTerminal);
	bool inFollow(const Terminal& terminal, const NonTerminal& nonTerminal);
	bool inSet(const Terminal& symbol, const TerminalSet& symbolSet);


};

#endif // !PARSER_H