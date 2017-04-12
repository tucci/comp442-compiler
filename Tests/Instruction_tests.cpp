#include "stdafx.h"
#include "CppUnitTest.h"
#include "../comp442_compilers/Instructions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
	TEST_CLASS(Instruction) {
public:

	TEST_METHOD(Instruction_LoadWord) {
		Register ri = r1;
		Register rj = r2;
		std::string k = "label";

		std::string expected = "\tlw\tr1,label(r2)\n";
		std::string actual = LoadWordInstruction(ri, rj, k)._toMoonCode();

		Assert::IsTrue(expected == actual);
	};

	TEST_METHOD(Instruction_StoreWord) {
		Register ri = r1;
		Register rj = r2;
		std::string k = "label";

		std::string expected = "\tsw\tlabel(r2),r1\n";
		std::string actual = StoreWordInstruction(rj, ri, k)._toMoonCode();

		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_MathOpCodeToString) {
		Assert::IsTrue("add" == opcodeToString(op_add));
		Assert::IsTrue("addi" == opcodeToString(op_addi));
		Assert::IsTrue("sub" == opcodeToString(op_sub ));
		Assert::IsTrue("subi" == opcodeToString(op_subi));
		Assert::IsTrue("mul" == opcodeToString(op_mul ));
		Assert::IsTrue("muli" == opcodeToString(op_muli));
		Assert::IsTrue("div" == opcodeToString(op_div ));
		Assert::IsTrue("divi" == opcodeToString(op_divi));
		Assert::IsTrue("ceq" == opcodeToString(op_ceq ));
		Assert::IsTrue("ceqi" == opcodeToString(op_ceqi));
		Assert::IsTrue("cne" == opcodeToString(op_cne ));
		Assert::IsTrue("cnei" == opcodeToString(op_cnei));
		Assert::IsTrue("clt" == opcodeToString(op_clt ));
		Assert::IsTrue("clti" == opcodeToString(op_clti));
		Assert::IsTrue("cle" == opcodeToString(op_cle ));
		Assert::IsTrue("clei" == opcodeToString(op_clei));
		Assert::IsTrue("cgt" == opcodeToString(op_cgt ));
		Assert::IsTrue("cgti" == opcodeToString(op_cgti));
		Assert::IsTrue("cge" == opcodeToString(op_cge ));
		Assert::IsTrue("cgei" == opcodeToString(op_cgei));		
	};

	TEST_METHOD(Instruction_tokenToMathOpCode) {
		Assert::IsTrue(op_clt == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::lt)));
		Assert::IsTrue(op_cle == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::lesseq)));
		Assert::IsTrue(op_cne == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::noteq)));
		Assert::IsTrue(op_cgt == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::gt)));
		Assert::IsTrue(op_cge == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::greateq)));
		Assert::IsTrue(op_ceq == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::comparison)));
		Assert::IsTrue(op_add == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::addop)));
		Assert::IsTrue(op_sub == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::subtractop)));
		Assert::IsTrue(op_mul == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::multop)));
		Assert::IsTrue(op_div == operatorStringToMathOpCode(Specification::REVERSE_TOKEN_MAP.at(TokenType::divop)));
		
	};


	TEST_METHOD(Instruction_ArithmeticInstruction) {
		Register ri = r1;
		Register rj = r2;
		std::string k = "label";

		
		std::string expected = "\tadd\tr1,r1,r2\n";
		std::string actual = AddInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tsub\tr1,r1,r2\n";
		actual = SubtractInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);



		expected = "\tmul\tr1,r1,r2\n";
		actual = MultiplyInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tdiv\tr1,r1,r2\n";
		actual = DivisionInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tceq\tr1,r1,r2\n";
		actual = EqualInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tcne\tr1,r1,r2\n";
		actual = NotEqualInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tclt\tr1,r1,r2\n";
		actual = LessThanInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tcle\tr1,r1,r2\n";
		actual = LessThanEqualInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);
		

		expected = "\tcgt\tr1,r1,r2\n";
		actual = GreaterThanInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tcge\tr1,r1,r2\n";
		actual = GreatherThanEqualInstruction(r1, r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);

	};

	TEST_METHOD(Instruction_ArithmeticImmediateInstruction) {
		
		std::string k = "3";


		std::string expected = "\taddi\tr1,r1,3\n";
		std::string actual = AddImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tsubi\tr1,r1,3\n";
		actual = SubtractImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);



		expected = "\tmuli\tr1,r1,3\n";
		actual = MultiplyImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tdivi\tr1,r1,3\n";
		actual = DivisionImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tceqi\tr1,r1,3\n";
		actual = EqualImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tcnei\tr1,r1,3\n";
		actual = NotEqualImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tclti\tr1,r1,3\n";
		actual = LessThanImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tclei\tr1,r1,3\n";
		actual = LessThanEqualImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);


		expected = "\tcgti\tr1,r1,3\n";
		actual = GreaterThanImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

		expected = "\tcgei\tr1,r1,3\n";
		actual = GreatherThanEqualImmediateInstruction(r1, r1, k).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

	};

	TEST_METHOD(Instruction_PutCInstruction) {
		std::string expected = "\tputc\tr1\n";
		std::string actual = PutCInstruction(r1).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

	};

	TEST_METHOD(Instruction_GetCInstruction) {
		std::string expected = "\tgetc\tr1\n";
		std::string actual = GetCInstruction(r1).setComment("")._toMoonCode();
		Assert::IsTrue(expected == actual);

	};

	TEST_METHOD(Instruction_BranchIfZeroInstruction) {
		std::string expected = "\tbz\tr1,label\n";
		std::string actual = BranchIfZeroInstruction(r1, "label")._toMoonCode();
		Assert::IsTrue(expected == actual);
	};

	TEST_METHOD(Instruction_BranchIfNotZeroInstruction) {
		std::string expected = "\tbnz\tr1,label\n";
		std::string actual = BranchIfNonZeroInstruction(r1, "label")._toMoonCode();
		Assert::IsTrue(expected == actual);
	};

	TEST_METHOD(Instruction_JumpInstruction) {
		std::string expected = "\tj\tlabel\n";
		std::string actual = JumpInstruction("label")._toMoonCode();
		Assert::IsTrue(expected == actual);
	};

	TEST_METHOD(Instruction_JumpRegisterInstruction) {
		std::string expected = "\tjr\tr1\n";
		std::string actual = JumpRegisterInstruction(r1)._toMoonCode();
		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_JumpAndLinkInstruction) {
		std::string expected = "\tjl\tr1,label\n";
		std::string actual = JumpAndLinkInstruction(r1, "label")._toMoonCode();
		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_JumpAndLinkRegisterInstruction) {
		std::string expected = "\tjlr\tr1,r2\n";
		std::string actual = JumpAndLinkRegisterInstruction(r1, r2)._toMoonCode();
		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_NoopInstruction) {
		std::string expected = "\tnop\n";
		std::string actual = NoopInstruction()._toMoonCode();
		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_HaltInstruction) {
		std::string expected = "\thlt\n";
		std::string actual = HaltInstruction()._toMoonCode();
		Assert::IsTrue(expected == actual);
	};

	TEST_METHOD(Instruction_AlignDirective) {
		std::string expected = "\talign\n";
		std::string actual = AlignDirective()._toMoonCode();
		Assert::IsTrue(expected == actual);
	};

	TEST_METHOD(Instruction_EntryDirective) {
		std::string expected = "\tentry\n";
		std::string actual = EntryDirective()._toMoonCode();
		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_DefineWordDirective1) {
		std::string expected = "\tlabel\tdw\t0\n";
		std::string actual = DefineWordDirective("label")._toMoonCode();
		Assert::IsTrue(expected == actual);
	};


	TEST_METHOD(Instruction_DefineWordDirective2) {
		// TODO array/objects
		// This is implemeted seperatly using test files
		
	};


	TEST_METHOD(Instruction_ClearRegisterInstruction) {
		std::string expected = "\tsub\tr1,r1,r1\t% Clear register\n";
		std::string actual = ClearRegisterInstruction(r1)._toMoonCode();
		Assert::IsTrue(expected == actual);

	};

	TEST_METHOD(Instruction_CommentInstruction) {
		std::string expected = "% ---------------- test ----------------\n";
		std::string actual = CommentInstruction("test")._toMoonCode();
		Assert::IsTrue(expected == actual);

	};

	TEST_METHOD(Instruction_ExpressionEvalulationInstruction) {
		// TODO implement all cases using literals,vars,functions,arrays
		// This is implemeted seperatly using test files
	};











	};
}