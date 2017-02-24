#include "stdafx.h"

Parser::Parser(Lexer* lexer, Grammar* grammar) {
	this->lexer = lexer;
	this->grammar = grammar;
	firstSet = GrammarFirstFollowSetGenerator::buildFirstSet(*grammar);
	followSet = GrammarFirstFollowSetGenerator::buildFollowSet(*grammar, firstSet);
	buildParseTable();
}


Parser::~Parser() {
}

bool Parser::parse() {
	bool error = false;
	parseStack.push_back(SpecialTerminal::END_OF_FILE);
	parseStack.push_back(grammar->getStartSymbol());
	std::string derivationString = parseStack.at(parseStack.size() - 1).getName();
	lookAheadToken = lexer->nextToken();
	addToDerivationList(getStackContents(), "", derivationString);
	while (parseStack.at(parseStack.size() - 1).getName() != SpecialTerminal::END_OF_FILE.getName()) {
		Symbol x = parseStack.at(parseStack.size() - 1);
		std::string stackContent = getStackContents();
		if (x.isTerminal()) {
			Terminal tx = static_cast<Terminal&>(x);
			addToDerivationList(getStackContents(), "", "");
			if (matchTerminalToTokenType(tx, lookAheadToken)) {
				parseStack.pop_back();
				lookAheadToken = lexer->nextToken();
			} else {
				skipErrors();
				error = true;
			}
		} else {
			// x is not a terminal so cast to non terminal
			NonTerminal nx = static_cast<NonTerminal&>(x);
			Terminal lookaheadTerminal = tokenToTerminal(lookAheadToken, nx);
			const Production p = parseTable.at(nx).at(lookaheadTerminal);
			if (p != Production::ERROR_PRODUCTION) {
				std::string preStackContents = getStackContents();
				parseStack.pop_back();
				inverseRHSMultiplePush(p, derivationString);
				addToDerivationList(preStackContents, p.toString(), derivationString);
			} else {
				skipErrors();
				error = true;
			}
		}
	}
	
	if (!matchTerminalToTokenType(SpecialTerminal::END_OF_FILE, lookAheadToken) || error == true) {
		return false;
	} else {
		derivationString = "success";
		addToDerivationList(getStackContents(), "", derivationString);
		return true;
	}
	return false;
}


void Parser::buildParseTable() {
	// Taken from power point
	const std::vector<std::shared_ptr<Production>> productions = grammar->getProductions();
	typedef std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> TerminalSetPtr;
	const TerminalSetPtr terminalSet = grammar->getTerminals();
	std::unordered_map <Terminal, Production, SymbolHasher, SymbolEqual> emptyRow;
	for (std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual>::const_iterator t = terminalSet.begin(); t != terminalSet.end(); ++t) {
		// Add this terminal to the row
		emptyRow.emplace(*(t->get()), Production::ERROR_PRODUCTION);
	}
	for (std::vector<std::shared_ptr<Production>>::const_iterator p = productions.begin(); p != productions.end(); ++p) {
		NonTerminal lhs = (*p)->getNonTerminal();
		std::vector<Symbol> rhs = (*p)->getProduction();
		parseTable.emplace(lhs, emptyRow);
		TerminalSet firstRhs = GrammarFirstFollowSetGenerator::computeFirst(rhs, firstSet);
		for (TerminalSetPtr::iterator t_ptr = terminalSet.begin(); t_ptr != terminalSet.end(); ++t_ptr) {
			Terminal t = *t_ptr->get();
			if (t.isTerminal() && !SpecialTerminal::isEpsilon(t.getName())) {
				Terminal terminalCast = static_cast<Terminal&>(t);
				if (inSet(terminalCast, firstRhs)) {
					// Revove the error production there
					parseTable.at(lhs).erase(terminalCast);
					// Add the updated production
					parseTable.at(lhs).emplace( terminalCast, *p->get());
				}
			}
		}
		if (inSet(SpecialTerminal::EPSILON, firstRhs)) {
			for (TerminalSetPtr::iterator t_ptr = terminalSet.begin(); t_ptr != terminalSet.end(); ++t_ptr) {
				Terminal t = *t_ptr->get();
				if (t.isTerminal()) {
					Terminal terminalCast = static_cast<Terminal&>(t);
					if (inFollow(terminalCast, lhs)) {
						// Revove the error production there
						parseTable.at(lhs).erase(terminalCast);
						// Add the updated production
						parseTable.at(lhs).emplace(terminalCast, *p->get());
					}
				}	
			}
		}
	}
}

void Parser::skipErrors() {
}

void Parser::inverseRHSMultiplePush(const Production& production, std::string& derivation) {
	std::vector<Symbol> rhs = production.getProduction();
	std::string replaceWith;
	std::string nonTerminalString = production.getNonTerminal().getName();
	int replaceIndex = derivation.find(nonTerminalString);
	int replaceLength = nonTerminalString.length();
	// This pushes the production in reverse order
	for (std::vector<Symbol>::reverse_iterator it = rhs.rbegin(); it != rhs.rend(); ++it) {
		// If this is epsilon, dont push it onto the stack. just continue on
		if (it->getName() == SpecialTerminal::EPSILON.getName()) {
			derivation.replace(replaceIndex, replaceLength, "");
			return;
		}
		parseStack.push_back(*it);
		replaceWith = it->getName() + " " + replaceWith;
	}
	
	derivation.replace(replaceIndex, replaceLength, replaceWith);
}

bool Parser::inFirst(const Terminal& terminal, const NonTerminal& nonTerminal)  {
	return inSet(terminal, firstSet.at(nonTerminal));
}

bool Parser::inFollow(const Terminal& terminal, const NonTerminal& nonTerminal) {
	return inSet(terminal, followSet.at(nonTerminal));
}

bool Parser::inSet(const Terminal& symbol, const TerminalSet& symbolSet) {
	TerminalSet::const_iterator got = symbolSet.find(symbol);
	if (got == symbolSet.end()) {
		return false;
	}
	return true;
}

bool Parser::matchTerminalToTokenType(const Terminal& terminal, const Token& token) {
	TokenType terminalTokenType = Specification::TOKEN_MAP.at(terminal.getName());
	// TODO: fix grammar. we have problem with float in indice. i.e n[2.1]
	// TODO: factor in num type
	if (terminalTokenType == TokenType::int_value && token.type == TokenType::num) {
		return true;
	}
	return terminalTokenType == token.type;
}

Terminal Parser::tokenToTerminal(const Token& token, const NonTerminal& nt) {
	
	if (Specification::isInteger(token)) {
		// Custom logic to update num to integer when we have an arithexprAEInt and sub productions
		// Really ugly and temp solution to this problem
		if (nt.getName().find("AEInt") != std::string::npos) {
			Terminal t(Specification::REVERSE_TOKEN_MAP.at(TokenType::int_value));
			return t;
		}
	}
	Terminal t(Specification::REVERSE_TOKEN_MAP.at(token.type));	
	return t;
}

void Parser::outputParserDataToFile() {
	std::ofstream firstfollowOutput;
	std::ofstream parsingTableOutput;

	firstfollowOutput.open("first_and_follow_sets.txt");

	firstfollowOutput << "First Set\n";
	for (auto nt_iterator : firstSet) {
		firstfollowOutput << "first(" << nt_iterator.first.getName() << ") : { ";
		int size = nt_iterator.second.size();
		int i = 0;
		for (auto t_iterator : nt_iterator.second) {
			firstfollowOutput << t_iterator.getName();
			// Prevent comma at end of set output
			if (i < size - 1) {
				firstfollowOutput << ", ";
			}
			i++;
		}
		firstfollowOutput << " }\n";
	}

	firstfollowOutput << "\nFollow Set\n";

	for (auto nt_iterator : followSet) {
		firstfollowOutput << "follow(" << nt_iterator.first.getName() << ") : { ";
		int size = nt_iterator.second.size();
		int i = 0;
		for (auto t_iterator : nt_iterator.second) {
			firstfollowOutput << t_iterator.getName();
			// Prevent comma at end of set output
			if (i < size - 1) {
				firstfollowOutput << ", ";
			}
			i++;
		}
		firstfollowOutput << " }\n";
	}
	firstfollowOutput.close();


	// Since printing a table would be a disaster in a text file
	// lets output to a html file
	parsingTableOutput.open("parsingtable.html");
	parsingTableOutput << "<html><head><style>table, th, td{border: 1px solid black;}ul{list-style-type: none;}</style></head><body>";
	parsingTableOutput << "<h1>Production list</h1>";
	// Print out rules
	std::vector<std::shared_ptr<Production>> productions = grammar->getProductions();
	// Get a map of production to indexes
	std::unordered_map<Production, int, ProductionHasher> productionToRuleIndexMap;

	int productionNumber = 1;
	parsingTableOutput << "<ul>";
	for (auto p : productions) {
		productionToRuleIndexMap.emplace(*p, productionNumber);
		parsingTableOutput << "<li>";
		parsingTableOutput << "r" << productionNumber << " : " << *p;
		parsingTableOutput << "</li>";
		productionNumber++;
	}
	parsingTableOutput << "</ul>";

	parsingTableOutput << "<h1>Parsing table</h1>";
	parsingTableOutput << "<table>";
	parsingTableOutput << "<tr>";
	parsingTableOutput << "<th>Non Terminal</th>";


	std::unordered_set<std::shared_ptr<Terminal>, SymbolHasher, SymbolEqual> terminals = grammar->getTerminals();

	// Get an indexable copy
	std::vector<std::shared_ptr<Terminal>> terminalVector(terminals.begin(), terminals.end());

	// Output header of table
	for (int sCol = 0; sCol < terminalVector.size(); sCol++) {
		parsingTableOutput << "<th>" << terminalVector.at(sCol)->getName() << "</th>";
	}
	parsingTableOutput << "</tr>"; // close the header row


	// Loop over every non terminal/row
	for (auto nt_iterator : parseTable) {
		parsingTableOutput << "<tr>";
		// Get the terminal map for this row

		// output the non terminal symbol for this row
		parsingTableOutput <<  "<td>" << nt_iterator.first.getName() << "</td>";
		// Loop over the terminals in the same order as our header
		for (int i = 0; i < terminalVector.size(); i++) {
			// Get the production for this terminal column
			Production columnProduction = nt_iterator.second.at(*terminalVector.at(i).get());
			if (columnProduction != Production::ERROR_PRODUCTION) {
				//parsingTableOutput << "<td>" <<  columnProduction  <<  " </td>";
				// Get the mapped rule index for this production
				parsingTableOutput << "<td>" <<  "r" <<  productionToRuleIndexMap.at(columnProduction) << " </td>";
			} else {
				parsingTableOutput << "<td></td>";
				// dont print anything for error productions
			}
		}
		// Close the row for this non terminal
		parsingTableOutput << "</tr>";
	}
	parsingTableOutput << "</table>"; // close table
	parsingTableOutput << "</body></html>"; // close html
	parsingTableOutput.close(); // close file
}

void Parser::outputAnalysis() {
	
	std::ofstream parserDerivation;
	// TODO: output errors as well
	std::ofstream parserErrors;


	parserDerivation.open("derivation.html");
	parserDerivation << "<html><head><style>table, th, td{border: 1px solid black;}ul{list-style-type: none;}</style></head><body>";
	parserDerivation << "<h1>Derivation</h1>";
	parserDerivation << "<table>";
	parserDerivation << "<tr>";

	parserDerivation << "<th></th>";
	parserDerivation << "<th>Stack</th>";
	parserDerivation << "<th>Production</th>";
	parserDerivation << "<th>Derivation</th>";

	parserDerivation << "</tr>";

	int derivationCount = 1;
	for (auto d : derivation) {
		parserDerivation << "<tr>";
		parserDerivation << "<td>" << derivationCount << "</td>";
		parserDerivation << "<td>" << d.stackContent << "</td>";
		parserDerivation << "<td>" << d.production << "</td>";
		parserDerivation << "<td>" << d.derivation << "</td>";
		parserDerivation << "</tr>";
		derivationCount++;
	}
	parserDerivation << "</table>"; // close table
	parserDerivation << "</body></html>"; // close html
	parserDerivation.close();
}

std::string Parser::getStackContents() {
	std::string contents;
	for (auto s : parseStack) {
		contents.append(s.getName() + " ");
	}
	return contents;
}

void Parser::addToDerivationList(const std::string& stackContents, const std::string& production, const std::string& derivationString) {
	derivation.push_back({ stackContents, production, derivationString });
}