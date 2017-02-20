#ifndef SYMBOL_H
#define SYMBOL_H


// An abstract class to hold whether this symbol is a terminal or non terminal symbol
class Symbol {

public:
	static const Symbol EPSILON;
	static const Symbol END_OF_FILE;
	

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


// Taken from http://stackoverflow.com/questions/15810620/unordered-map-with-custom-hashing-equal-functions-functions-dont-get-called
class SymbolHasher {
public:
	size_t operator() (const std::shared_ptr<Symbol> key) const {
		std::string stringKey = key.get()->getName();
		size_t hash = 0;
		for (size_t i = 0; i< stringKey.size(); i++) {
			hash += (71 * hash + stringKey[i]) % 5;
		}
		return hash;
	}
};
class SymbolEqual {
public:
	bool operator() (const std::shared_ptr<Symbol> s1, const std::shared_ptr<Symbol>& s2) const {

		std::string s1Name = s1.get()->getName();
		std::string s2Name = s2.get()->getName();
		return !(s1Name.compare(s2Name));
	}
};

#endif // !SYMBOL_H

