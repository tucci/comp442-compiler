#ifndef PARSER_H
#define PARSER_H


class Parser {
public:
	// Create a parser from a given lexer and grammar
	Parser(Lexer* lexer, Grammar* grammar);
	~Parser();
	// Parse method
	bool parse();
	// Outputs the data about the fisrt/follow sets and parsing table
	void outputParserDataToFile();
private:
	// Our lexer
	Lexer* lexer;
	// The grammar our parser is trying to validate
	Grammar* grammar;
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
	void inverseRHSMultiplePush(const Production& production);

	// Returns true if the terminal is in the first set for the given non terminal production
	bool inFirst(const Terminal& terminal, const NonTerminal& nonTerminal);
	// Returns true if the terminal is in the follow set for the given non terminal production
	bool inFollow(const Terminal& terminal, const NonTerminal& nonTerminal);
	// Returns true if the terminal is in the given terminal set
	bool inSet(const Terminal& symbol, const TerminalSet& symbolSet);
	static bool matchTerminalToTokenType(const Terminal& terminal, const Token& token);
	// Creates the tokens complementary terminal
	Terminal tokenToTerminal(const Token& token);

};

#endif // !PARSER_H