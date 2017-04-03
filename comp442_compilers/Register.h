#ifndef REGISTER_H
#define REGISTER_H

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
	r14,
	r15,
};

static std::string ToRegisterValue(Register r) {
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
#endif

