#ifndef TERMINAL_H
#define TERMINAL_H

// A concrete class to hold terminal symbols
class Terminal : public Symbol {
public:
	Terminal(std::string name);
	~Terminal();
};


namespace std {
	template<>
	struct hash<Terminal> {
		size_t
			operator()(const Terminal& s) const {
			return std::hash<string>()(s.getName());
		}
	};
}


inline bool operator==(const Terminal& p1, const Terminal& p2) {
	return p1.getName() == p2.getName();
}


#endif