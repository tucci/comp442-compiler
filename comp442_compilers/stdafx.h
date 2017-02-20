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

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>

// Our classes
#include "token.h"
#include "dfa.h"
#include "specification.h"
#include "lexer.h"
// Helper classes
#include "StringUtils.h"


// Grammar
#include "Symbol.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "Production.h"
#include "Grammar.h"
#include "Parser.h"
#include "Compiler.h"

#endif // !STDAFX_H
