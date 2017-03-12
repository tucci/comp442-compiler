#ifndef PRODUCTION_H
#define PRODUCTION_H

class Production {
public:
	static const NonTerminal ERROR_PRODUCTION_NON_TERMINAL;
	static const Production ERROR_PRODUCTION;
	// Creates a production from a given non terminal symbol, and list of symbols for a production
	Production(const NonTerminal& nonTerminal, const std::vector<Symbol>& production);
	~Production();
	// Gets the lhs for this production
	NonTerminal getNonTerminal() const;
	// Gets the rhs for this production without sdt
	std::vector<Symbol> getProduction() const;
	// Gets the rhs for this production with sdt
	std::vector<Symbol> getProductionSDT() const;
	// To string for production without sdt
	std::string toString() const;
	// Overide << for easier print outs
	friend std::ostream& operator<<(std::ostream& os, const Production& p);
	friend bool operator==(const Production& p1, const Production& p2);
	friend bool operator!=(const Production& p1, const Production& p2);
private:
	// the lhs non terminal
	const NonTerminal mNonTerminal;
	// the rhs for this production without the semantic symbols
	const std::vector<Symbol> mProduction;
	// the rhs for this production with the semantic symbols
	const std::vector<Symbol> mProductionWithSDT;
};

// Used when printing out the table
class ProductionHasher {
public:
	// Used when we have sets/maps with shared pointers
	size_t operator() (const std::shared_ptr<Production> p) const {
		std::string stringKey = p->getNonTerminal().getName();
		for (auto s : p->getProduction()) {
			stringKey.append(s.getName()  + " ");
		}
		return hashFunc(stringKey);
	}
	// Used when we have sets/maps with non pointer data
	size_t operator() (const Production& p) const {
		std::string stringKey = p.getNonTerminal().getName();
		for (auto s : p.getProduction()) {
			stringKey.append(s.getName() + " ");
		}
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
#endif