#ifndef PARSER_H
#define PARSER_H

// A simple stucture to hold data about a single derivaton
struct DerivationData {
	std::string stackContent;
	std::string production;
	std::string derivation;
	std::string toString() {
		return stackContent + "\t\t" + production + "\t\t" + derivation + "\n";
	};
};

struct SyntaxError {
	int tokenPosition;
	Token token;
	Token lookaheadToken;
};

class Parser {
	friend class ParserGenerator;
public:
	~Parser();
	// Parse method 
	bool parse();
	// Outputs the data about the fisrt/follow sets and parsing table
	void outputParserDataToFile();
	// Outputs the derivation and any errors
	void outputAnalysis();
private:
	// Parser Generator will create this parser for us
	Parser();
	// Our lexer
	Lexer* lexer;
	// The grammar our parser is trying to validate
	Grammar* grammar;
	// The token before the lookahead token. This is used during error recovery
	// Since we only find out the error after we consumed the token, we need to keep track of the token we just consumed
	Token consumedToken;
	// The lookahead token for our parser to use
	Token lookAheadToken;

	// Global symbol table
	SymbolTable symbolTable;
	// Current symbol table we are working in
	SymbolTable* currentSymbolTable;
	// The current semantic context that holds the data for the current parse
	SemanticContext context;

	// Data structure to hold first set
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> firstSet;
	// Data structure to hold follow set
	std::unordered_map<NonTerminal, TerminalSet, SymbolHasher, SymbolEqual> followSet;
	// Data structe to hold the parse table
	std::unordered_map <NonTerminal, TerminalToProductionMap, SymbolHasher, SymbolEqual>  parseTable;
	// Data structure to hold our parse stack
	std::vector<Symbol> parseStack;
	// stack that holds the semantic actions
	// TODO: delete if not needed
	std::vector<SemanticSymbol> semanticStack;
	// The list of derivations while parsing
	std::vector<DerivationData> derivation;
	// The index of the current token
	int tokenIndex;
	// The list of syntax errors during parsing
	std::vector<SyntaxError> errors;
	// Internally calls the lexer
	void nextToken();
	// Handles the errors
	void skipErrors();
	// Pushes the rhs of this production in inverse order
	void inverseRHSMultiplePush(const Production& production, std::string& derivation);
	// Matches the terminal to the token from our lexer
	static bool matchTerminalToTokenType(const Terminal& terminal, const Token& token);
	// Creates the tokens complementary terminal depending on the non terminal. Used to convert num to integer sometimes
	Terminal tokenToTerminal(const Token& token, const NonTerminal& nt);
	// Gets the contents of the stack when called
	std::string getStackContents();
	// Adds a derivation to the current derivation list
	void addToDerivationList(const std::string& stackContents, const std::string& production, const std::string& derivationString);
	// Helper method to add errors to our error list
	void addError();
};

#endif // !PARSER_H