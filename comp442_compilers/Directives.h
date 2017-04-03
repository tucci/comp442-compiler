#ifndef DIRECTIVES_H
#define DIRECTIVES_H
#include "Directive.h"
#include <string>


class DW_Directive : public Directive {
public:

	DW_Directive(SymbolTable* globalTable, SymbolTableRecord* record) : Directive(globalTable) {
		this->record = record;
	}

	std::string _toMoonCode() override {
		// Create the entry in the moon code
		if (record->typeStructure.structure == SymbolStructure::struct_array || record->typeStructure.type == SymbolType::type_class) {
			int allocationSize = SymbolTable::_sizeOf(globalTable, record->typeStructure);
			return record->label + "\t" + "res" + "\t" + std::to_string(allocationSize);

		} else {
			// Used for simple primative types ex simple int/ simple float
			// Ex	x	dw	0
			return record->label + "\t" + "dw" + "\t" + "0";
		}
	};

private:
	SymbolTableRecord* record;
};
#endif

