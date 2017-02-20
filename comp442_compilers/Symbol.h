#ifndef SYMBOL_H
#define SYMBOL_H


// An abstract class to hold whether this symbol is a terminal or non terminal symbol
class Symbol {

public:
	

	~Symbol();
	// Get the name of this symbol
	const std::string& getName() const;
	// Get if this symbol is a terminal symbol
	bool isTerminal();
	// Overide << for easier print outs
	friend std::ostream& operator<<(std::ostream& os, const Symbol& s);


protected:
	// Constructor as protected to allow subclasses to override this
	// this is an abstract class
	Symbol(std::string name, bool isTerminal);
	// The name of this symbol
	std::string mName;
	// Whether this symbol is a terminal symbol
	bool mIsTerminalSymbol;
};
#endif // !SYMBOL_H
