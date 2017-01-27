// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef STDAFX_H
#define STDAFX_H


// TODO: tabs at the end of files create new tokensode
// TODO: create instructions and documenation. this includes lexicial errors and how it recovers
// TODO: see if u need targetver.h
#include "targetver.h"

// external libs
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <fstream>

// Our files
#include "token.h"
#include "dfa.h"
#include "specification.h"
#include "lexer.h"


#endif // !STDAFX_H