#ifndef NONTERMINAL_H
#define NONTERMINAL_H

// A concrete class to hold non terminal symbols
class NonTerminal : public Symbol {
public:
	NonTerminal(std::string name);
	~NonTerminal();
};

namespace std {
	template<>
	struct hash<NonTerminal> {
		size_t
			operator()(const NonTerminal& s) const {
			return std::hash<string>()(s.getName());
		}
	};
}

inline bool operator==(const NonTerminal& p1, const NonTerminal& p2) {
	return p1.getName() == p2.getName();
}

#endif 