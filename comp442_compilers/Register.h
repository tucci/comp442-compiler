#ifndef REGISTER_H
#define REGISTER_H


// Registers are used during the code generation phase
// This enum allows us to abstract away code register valuees
enum Register {
	r0, // Zero
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	r8,
	r9,
	r10,
	r11,
	r12,
	r13,
	r14, // r14 is used for the function return value
	r15, // r15 is used for function linking
	r_error // used for when there is no more registers
};

// Converts Register enum to its corresponding register value string
// Example the register r2 will be converted to "r2".
// Used to convert register object to register string
static std::string registerToString(Register r) {
	switch (r) {
		case r0: return "r0";
		case r1: return "r1";
		case r2: return "r2";
		case r3: return "r3";
		case r4: return "r4";
		case r5: return "r5";
		case r6: return "r6";
		case r7: return "r7";
		case r8: return "r8";
		case r9: return "r9";
		case r10: return "r10";
		case r11: return "r11";
		case r12: return "r12";
		case r13: return "r13";
		case r14: return "r14";
		case r15: return "r15";
		default: ;
	}
}

// Converts a register value string to a register enum
static Register stringToRegister(std::string r) {
	if (r == "r0") return r0;
	else if (r == "r1") return r1;
	else if (r == "r2") return r2;
	else if (r == "r3") return r3;
	else if (r == "r4") return r4;
	else if (r == "r5") return r5;
	else if (r == "r6") return r6;
	else if (r == "r7") return r7;
	else if (r == "r8") return r8;
	else if (r == "r9") return r9;
	else if (r == "r10") return r10;
	else if (r == "r11") return r11;
	else if (r == "r12") return r12;
	else if (r == "r13") return r13;
	else if (r == "r14") return r14;
	else if (r == "r15") return r15;
	
}
#endif

