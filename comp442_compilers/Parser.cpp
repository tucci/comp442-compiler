#include "stdafx.h"


Parser::Parser() {
	symbolTable.tableTagName = "Global";
	currentSymbolTable = &symbolTable;
}


Parser::~Parser() {
}

bool Parser::parse() {
	bool error = false;
	parseStack.push_back(SpecialTerminal::END_OF_FILE);
	parseStack.push_back(grammar->getStartSymbol());
	std::string derivationString = parseStack.at(parseStack.size() - 1).getName();
	nextToken();
	addToDerivationList(getStackContents(), "", derivationString);
	while (parseStack.at(parseStack.size() - 1).getName() != SpecialTerminal::END_OF_FILE.getName()) {
		Symbol x = parseStack.at(parseStack.size() - 1);
		std::string stackContent = getStackContents();
		// If this is a terminal match it
		if (x.isTerminal()) {
			Terminal tx = static_cast<Terminal&>(x);
			addToDerivationList(getStackContents(), "", "");
			if (matchTerminalToTokenType(tx, lookAheadToken)) {
				parseStack.pop_back();
				nextToken();
			} else {
				skipErrors();
				error = true;
			}
		} else {
			
			if (x.isSemantic()) {
				SemanticSymbol semanticSymbol = static_cast<SemanticSymbol&>(x);
				SemanticActions::performAction(semanticSymbol, semanticStack, &currentSymbolTable, consumedToken);
				parseStack.pop_back();
			} else {
				// This is not a terminal. So it is a non terminal
				NonTerminal nx = static_cast<NonTerminal&>(x); // x is not a terminal so cast to non terminal
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

void Parser::nextToken() {
	tokenIndex++;
	consumedToken = lookAheadToken;
	lookAheadToken = lexer->nextToken();
}

void Parser::skipErrors() {
	Symbol top = parseStack.at(parseStack.size() - 1);
	int loopCount = 0;
	if (!top.isTerminal()) {
		NonTerminal ntTop = static_cast<NonTerminal&>(top);
		Terminal lookAheadTerminal = tokenToTerminal(lookAheadToken, ntTop);

		if (lookAheadToken.type == TokenType::end_of_file_token || ParserGenerator::inFollow(lookAheadTerminal, ntTop, followSet) && parseTable.at(ntTop).at(lookAheadTerminal) == Production::ERROR_PRODUCTION) {
				parseStack.pop_back();
		} else {
			while (!ParserGenerator::inFirst(lookAheadTerminal, ntTop, firstSet) && !ParserGenerator::inFollow(lookAheadTerminal, ntTop, followSet)){
				if (parseTable.at(ntTop).at(lookAheadTerminal) == Production::ERROR_PRODUCTION) {
					loopCount++;
					// Hack: Avoids infinite loops
					// This "should" rarely happen 
					if (loopCount == 1000) {
						// Output which ever errors we alreay have						
						outputAnalysis();
						// Throw expection so that main could catch and deallocate our memory
						std::cout << "There was an unrecoverable error during parsing the file." << std::endl;
						throw std::exception("There was an unrecoverable error during parsing the file.");
					}
					nextToken();
					lookAheadTerminal = tokenToTerminal(lookAheadToken, ntTop);
				} else {
					break;
				}
			}
		}
	} else {
		// This is a terminal, so lets pop it and continue
		parseStack.pop_back();
	}
	// Add the error we just found to the list
	addError();
}

void Parser::inverseRHSMultiplePush(const Production& production, std::string& derivation) {
	std::vector<Symbol> rhs = production.getProductionSDT();
	std::string replaceWith;
	std::string nonTerminalString = production.getNonTerminal().getName();
	size_t replaceIndex = derivation.find(nonTerminalString);
	size_t replaceLength = nonTerminalString.length();
	// This pushes the production in reverse order
	for (std::vector<Symbol>::reverse_iterator it = rhs.rbegin(); it != rhs.rend(); ++it) {
		// If this is epsilon, dont push it onto the stack. just continue on
		if (it->getName() == SpecialTerminal::EPSILON.getName()) {
			derivation.replace(replaceIndex, replaceLength, "");
			return;
		}
		parseStack.push_back(*it);
		// Dont add semantic values to the derivtion
		if (!it->isSemantic()) {
			replaceWith = it->getName() + " " + replaceWith;
		}
		
	}
	
	derivation.replace(replaceIndex, replaceLength, replaceWith);
}

bool Parser::matchTerminalToTokenType(const Terminal& terminal, const Token& token) {
	TokenType terminalTokenType = Specification::TOKEN_MAP.at(terminal.getName());
	if (Specification::isInteger(token) &&  token.type == TokenType::num) {
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
		size_t size = nt_iterator.second.size();
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
		size_t size = nt_iterator.second.size();
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

	std::ofstream parserErrors;
	parserErrors.open("parserErrors.txt");

	for (SyntaxError error : errors) {		
		parserErrors << "Syntax error on line " << error.token.tokenLine << ". Unexpected identifer \"" << error.lookaheadToken.lexeme << "\"";
		if (error.token.type != TokenType::non_token) {
			parserErrors << " after \"" << error.token.lexeme << "\"";
		}
		parserErrors << std::endl;
	}

	parserErrors.close();
	// Output symbol table
	std::cout << symbolTable.toString();
}

std::string Parser::getStackContents() {
	std::string contents;
	for (auto s : parseStack) {
		if (!s.isSemantic()) {
			contents.append(s.getName() + " ");
		}
		
	}
	return contents;
}

void Parser::addToDerivationList(const std::string& stackContents, const std::string& production, const std::string& derivationString) {
	derivation.push_back({ stackContents, production, derivationString });
}

void Parser::addError() {
	SyntaxError error = { tokenIndex , consumedToken, lookAheadToken };

	bool isDuplicate;

	// Logic to avoid outputing duplcate errors
	if (errors.size() == 0) {
		isDuplicate = false;
	} else {
		// Compare the top error with the current error
		SyntaxError topError = errors.at(errors.size() - 1);
		isDuplicate = error.tokenPosition == topError.tokenPosition
			&& error.token.tokenLine == topError.token.tokenLine
			&& error.token.lexeme == topError.token.lexeme;
	}

	// If this is not a duplicate error, add it to the error list
	if (!isDuplicate) {
		// Add the error to the error list
		errors.push_back(error);
	}
}

