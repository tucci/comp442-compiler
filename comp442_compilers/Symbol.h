#ifndef SYMBOL_H
#define SYMBOL_H


// An abstract class to hold whether this symbol is a terminal or non terminal symbol
class Symbol {

public:

	~Symbol();
	// Get the name of this symbol
	const std::string& getName() const;
	// Get if this symbol is a terminal symbol
	bool isTerminal() const;
	// Get if this symbol is a semantic symbol
	bool isSemantic() const;
	// Overide << for easier print outs
	friend std::ostream& operator<<(std::ostream& os, const Symbol& s);

protected:
	// Constructor as protected to allow subclasses to override this
	// this is an abstract class
	Symbol(std::string name, bool isTerminal=false, bool isSemantic=false);
	// The name of this symbol
	std::string mName;
	// Whether this symbol is a terminal symbol
	bool mIsTerminalSymbol;
	// Whether this symbol is a semantic symbol
	bool mIsSemanticSymbol;
};


// Taken from http://stackoverflow.com/questions/15810620/unordered-map-with-custom-hashing-equal-functions-functions-dont-get-called
class SymbolHasher {
public:
	// Used when we have sets/maps with shared pointers
	size_t operator() (const std::shared_ptr<Symbol> key) const {
		const std::string stringKey = key.get()->getName();
		return hashFunc(stringKey);
	}
	// Used when we have sets/maps with non pointer data
	size_t operator() (const Symbol& key) const {
		const std::string stringKey = key.getName();
		return hashFunc(stringKey);
	}

private:
	// The base hashing function
	size_t hashFunc(const std::string& key) const {
		size_t hash = 0;
		for (size_t i = 0; i< key.size(); i++) {
			hash += (71 * hash + key[i]) % 5;
		}
		return hash;
	}
};
class SymbolEqual {
public:
	// Used when we have sets/maps with shared pointers
	bool operator() (const std::shared_ptr<Symbol> s1, const std::shared_ptr<Symbol>& s2) const {
		return equal(s1->getName(), s2->getName());
	}

	// Used when we have sets/maps with non pointer data
	bool operator() (const Symbol& s1, const Symbol& s2) const {
		return equal(s1.getName(), s2.getName());
	}
private:
	// The base equal function
	bool equal(const std::string& s1, const std::string& s2) const {
		return !(s1.compare(s2));
	}
};

#endif // !SYMBOL_H

