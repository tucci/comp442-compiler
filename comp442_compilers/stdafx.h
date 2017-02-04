// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STDAFX_H
#define STDAFX_H

#include "targetver.h"

// external libs
#include <string>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <memory>
#include <fstream>

// Our files
#include "token.h"
#include "dfa.h"
#include "specification.h"
#include "lexer.h"

// Grammar
#include "Symbol.h"
#include "Terminal.h"
#include "NonTerminal.h"


#endif // !STDAFX_H
