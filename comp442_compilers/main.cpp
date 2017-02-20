#include "stdafx.h"


using namespace std;
#include <string>
#include <iostream>
#include <sstream>


void createCode(std::string filename) {
	
	std::ifstream inputFile;
	try {
		inputFile.open(filename);
	

		std::stringstream inputStream;
		inputStream << inputFile.rdbuf();
		inputFile.close();

		// Read the first line to get the type
		std::string line;
		while (std::getline(inputStream, line, '\n')) {
			int splitIndex = line.find("->");
			std::string splitLine = line.substr(0, splitIndex);
			if (!splitLine.empty()) {
				cout << "NonTerminal " << splitLine << " =" << "g.addNonTerminal(\"" << splitLine << "\");" << endl;

			}
			
		}
		
		
	} catch (std::ifstream::failure e) {
		std::cout << "Error reading sample file";
	}

}




int main() {

	//createCode("grammar.txt");

	//bool writeToFile = false;
	//ofstream output;
	//ofstream error;

	//if (writeToFile) {
	//	output.open("output.txt");
	//	error.open("errors.txt");
	//}

	//Specification spec(true);
	//Lexer lex(&spec);
	//lex.setSource("SourceCode.txt");

	//while (lex.hasMoreTokens()) {
	//	Token token = lex.nextToken();
	//
	//	if (writeToFile) {
	//		// write to output/error file
	//		if (token.type == TokenType::error_token) {
	//			error << token;
	//		} else {
	//			output << token;
	//		}
	//	} else {			
	//		cout << token;
	//	}
	//}


	//if (writeToFile) {
	//	output.close();
	//	error.close();
	//	std::cout << "Successfully Wrote to output.txt and errors.txt" << std::endl;
	//}




	
	//NonTerminal s("S");
	//Terminal t("t");
	//std::vector<Symbol> startProduction = { t, s };
	//Production start(s,startProduction);

	//Grammar g(start);

	//// TODO: generate grammar from json file
	//NonTerminal term("<term>");
	//NonTerminal multop("<multop>");
	//NonTerminal factor("<factor>");
	//std::vector<Symbol> termRhs = { term, multop, factor };
	//Production termProduction(term, termRhs);
	//g.addProduction(termProduction);
	//cout << g << endl;
	
	// NonTerminals
	//NonTerminal E("E");
	//NonTerminal EPrime("E'");
	//NonTerminal T("T");
	//NonTerminal TPrime("T'");
	//NonTerminal F("F");

	//// Terminals
	//Terminal star("*");
	//Terminal plus("+");
	//Terminal zero("0");
	//Terminal one("1");
	//Terminal openparTerminal("(");
	//Terminal closeparTerminal(")");
	//Terminal epsilon("epsilon");

	// Productions
	/*std::vector<Symbol> r1RHS = { T, EPrime };
	Production r1(E, r1RHS);

	std::vector<Symbol> r2RHS = { plus, T, EPrime };
	Production r2(EPrime, r2RHS);

	std::vector<Symbol> r3RHS = { epsilon };
	Production r3(EPrime, r3RHS);

	std::vector<Symbol> r4RHS = { F, TPrime };
	Production r4(T, r4RHS);

	std::vector<Symbol> r5RHS = { star, F, TPrime };
	Production r5(TPrime, r5RHS);

	std::vector<Symbol> r6RHS = { epsilon };
	Production r6(TPrime, r6RHS);

	std::vector<Symbol> r7RHS = { zero };
	Production r7(F, r7RHS);

	std::vector<Symbol> r8RHS = { one };
	Production r8(F, r8RHS);


	std::vector<Symbol> r9RHS = { openparTerminal, E, closeparTerminal };
	Production r9(F, r9RHS);
	

	Grammar g(r1);
	g.addProduction(r2);
	g.addProduction(r3);
	g.addProduction(r4);
	g.addProduction(r5);
	g.addProduction(r6);
	g.addProduction(r7);
	g.addProduction(r8);
	g.addProduction(r9);
	cout << g << endl;*/

	Grammar g;

	Terminal classTerminal = g.addTerminal("class");
	Terminal idTerminal = g.addTerminal("id");
	Terminal openbraceTerminal = g.addTerminal("{");
	Terminal closebraceTerminal = g.addTerminal("}");
	Terminal semicolonTerminal = g.addTerminal(";");
	Terminal programTerminal = g.addTerminal("program");
	Terminal openparenTerminal = g.addTerminal("(");
	Terminal closeparenTerminal = g.addTerminal(")");
	Terminal forTerminal = g.addTerminal("for");
	Terminal ifTerminal = g.addTerminal("if");
	Terminal thenTerminal = g.addTerminal("then");
	Terminal getTerminal = g.addTerminal("get");
	Terminal putTerminal = g.addTerminal("put");
	Terminal returnTerminal = g.addTerminal("return");
	Terminal elseTerminal = g.addTerminal("else");
	Terminal dotTerminal = g.addTerminal(".");
	Terminal plusTerminal = g.addTerminal("+");
	Terminal minusTerminal = g.addTerminal("-");
	Terminal numTerminal = g.addTerminal("num");
	Terminal notTerminal = g.addTerminal("not");
	Terminal openSquareTerminal = g.addTerminal("[");
	Terminal closeSquareTerminal = g.addTerminal("]");
	Terminal integerTerminal = g.addTerminal("integer");
	Terminal floatTerminal = g.addTerminal("float");
	Terminal intTerminal = g.addTerminal("int");
	Terminal commaTerminal = g.addTerminal(",");
	Terminal equalTerminal = g.addTerminal("=");
	Terminal lessThanTerminal = g.addTerminal("<");
	Terminal greaterThanTerminal = g.addTerminal(">");
	Terminal orTerminal = g.addTerminal("or");
	Terminal andTerminal = g.addTerminal("and");
	Terminal multTerminal = g.addTerminal("*");
	Terminal divTerminal = g.addTerminal("/");

	NonTerminal prog = g.addNonTerminal("prog", true);
	NonTerminal classDecl = g.addNonTerminal("classDecl");
	NonTerminal classDeclList = g.addNonTerminal("classDeclList");
	NonTerminal progBody = g.addNonTerminal("progBody");

	NonTerminal funcDef = g.addNonTerminal("funcDef");
	NonTerminal funcDefRest = g.addNonTerminal("funcDefRest");
	NonTerminal funcDefList = g.addNonTerminal("funcDefList");
	NonTerminal funcBody = g.addNonTerminal("funcBody");

	NonTerminal varDecl = g.addNonTerminal("varDecl");
	NonTerminal varDeclNoId = g.addNonTerminal("varDeclNoId");
	NonTerminal varDeclRest = g.addNonTerminal("varDeclRest");
	NonTerminal varFuncDefList = g.addNonTerminal("varFuncDefList");
	NonTerminal varFuncDefListRest = g.addNonTerminal("varFuncDefListRest");
	
	NonTerminal varStatementList = g.addNonTerminal("varStatementList");
	NonTerminal varStatementListRest = g.addNonTerminal("varStatementListRest");

	NonTerminal statement = g.addNonTerminal("statement");
	NonTerminal statementList = g.addNonTerminal("statementList");
	NonTerminal noIdStartStatement = g.addNonTerminal("noIdStartStatement");
	NonTerminal elseBlock = g.addNonTerminal("elseBlock");	
	NonTerminal assignStat = g.addNonTerminal("assignStat");	
	NonTerminal assignStatRest = g.addNonTerminal("assignStatRest");
	NonTerminal assignStatRestRest = g.addNonTerminal("assignStatRestRest");
	NonTerminal statBlock = g.addNonTerminal("statBlock");
	
	NonTerminal expr = g.addNonTerminal("expr");
	NonTerminal exprRest = g.addNonTerminal("exprRest");
	NonTerminal relExpr = g.addNonTerminal("relExpr");
	NonTerminal arithExpr = g.addNonTerminal("arithExpr");
	NonTerminal arithExprRest = g.addNonTerminal("arithExprRest");
	NonTerminal sign = g.addNonTerminal("sign");
	
	NonTerminal term = g.addNonTerminal("term");
	NonTerminal termRest = g.addNonTerminal("termRest");
	NonTerminal factor = g.addNonTerminal("factor");
	NonTerminal factorRest = g.addNonTerminal("factorRest");
	NonTerminal factorRestRest = g.addNonTerminal("factorRestRest");
	
	NonTerminal variable = g.addNonTerminal("variable");
	NonTerminal dotidnest = g.addNonTerminal("dotidnest");
	NonTerminal dotidIndice = g.addNonTerminal("dotidIndice");
	NonTerminal dotidIndiceList = g.addNonTerminal("dotidIndiceList");
	
	NonTerminal indice = g.addNonTerminal("indice");
	NonTerminal indiceList = g.addNonTerminal("indiceList");
	NonTerminal arraySize = g.addNonTerminal("arraySize");
	NonTerminal arraySizeList = g.addNonTerminal("arraySizeList");
	NonTerminal typeNoId = g.addNonTerminal("typeNoId");
	NonTerminal type = g.addNonTerminal("type");
	NonTerminal fParams = g.addNonTerminal("fParams");
	NonTerminal fParamsTail = g.addNonTerminal("fParamsTail");
	NonTerminal fParamsTailList = g.addNonTerminal("fParamsTailList");
	NonTerminal aParams = g.addNonTerminal("aParams");
	NonTerminal aParamsTail = g.addNonTerminal("aParamsTail");
	NonTerminal aParamsTailList = g.addNonTerminal("aParamsTailList");
	NonTerminal assignOp = g.addNonTerminal("assignOp");
	NonTerminal relOp = g.addNonTerminal("relOp");
	NonTerminal relOpLessThanRest = g.addNonTerminal("relOpLessThanRest");
	NonTerminal relOpGreaterThanRest = g.addNonTerminal("relOpGreaterThanRest");
	NonTerminal addOp = g.addNonTerminal("addOp");
	NonTerminal multOp = g.addNonTerminal("multOp");

	g.addProduction(prog, { classDeclList, progBody });

	cout << g;


	
	

	


    return 0;
}






