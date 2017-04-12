#ifndef PARSER_GENERATOR_H
#define PARSER_GENERATOR_H



// A parser generator builds a LL1 table parser using the given lexer and grammar
class ParserGenerator {
public:
	~ParserGenerator();
	// Builds the parser from the given lexer and grammar
	static Parser* buildParser(Lexer* lexer, Grammar* grammar);

	// Creates an empty map that holds all the non terminals to empty sets
	static std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> initMap(const Grammar& g);

	// Computes an iteration of the first set, this needs to be called until no more new symbols are created
	static TerminalSet computeFirst(const std::vector<Symbol>& symbols,
		const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& first);

	// Builds the first set for this grammar
	static std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> buildFirstSet(const Grammar&);

	// Builds the follow set for this grammar
	static std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> buildFollowSet(const Grammar&,
		const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& firstSet);

	// Builds the parse table for this grammar
	static std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> buildParseTable(const Grammar&);

	// Builds the parse table for this grammar, where the first/follow set are already computed
	static std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual> buildParseTable(const Grammar&,
		const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& firstSet,
		const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& followSet
		);

	// Convient methods. Though these functions do the same thing, they provide a clean name to see if they are in the first
	// Returns true if the terminal is in the first set for the given non terminal production
	static bool inFirst(const Terminal& terminal, const NonTerminal& nonTerminal,
		const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& firstSet);

	// Returns true if the terminal is in the follow set for the given non terminal production
	static bool inFollow(const Terminal& terminal, const NonTerminal& nonTerminal,
		const std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual>& followSet);
private: 
	ParserGenerator();

};

#endif // !PARSER_GENERATOR_H