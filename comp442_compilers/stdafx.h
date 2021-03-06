// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STDAFX_H
#define STDAFX_H

#include "targetver.h"

// external libs
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <iostream>
#include <stdio.h>


#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>

// Lexer
#include "token.h"
#include "dfa.h"
#include "specification.h"
#include "lexer.h"



// Grammar
#include "Symbol.h"
#include "Terminal.h"
#include "SemanticSymbol.h"
#include "NonTerminal.h"
#include "Production.h"
#include "Grammar.h"

// Semantic data
// TODO: move this next to the code gen includes
#include "Instruction.h"

#include "TypeData.h"
#include "Variable.h"
#include "Expression.h"
#include "Statement.h"

#include "StatementBlock.h"
#include "AssignStatement.h"
#include "ForStatement.h"
#include "GetStatement.h"
#include "IfElseStatement.h"
#include "PutStatement.h"
#include "ReturnStatement.h"
#include "Attribute.h"



#include "SymbolTableRecord.h"
#include "SymbolTable.h"
#include "SemanticActions.h"

// Typedef this huge monster when building the first/follow sets
typedef std::unordered_set<Terminal, SymbolHasher, SymbolEqual> TerminalSet;
typedef std::unordered_map <Terminal, Production, SymbolHasher, SymbolEqual> TerminalToProductionMap;

// Util code
#include "Utils.h"

#include "MoonGenerator.h"

#include "Parser.h"
#include "ParserGenerator.h"
#include "Compiler.h"




#endif // !STDAFX_H
