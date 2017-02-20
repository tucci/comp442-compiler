#ifndef TERMINAL_H
#define TERMINAL_H

// A concrete class to hold terminal symbols
class Terminal : public Symbol {
public:
	Terminal(std::string name);
	~Terminal();
};



#endif