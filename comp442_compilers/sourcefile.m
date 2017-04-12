% ---------------- Allocate temp memory ----------------
	t28	dw	0
	t8	dw	0
	t20	dw	0
	t0	dw	0
	t21	dw	0
	t1	dw	0
	t22	dw	0
	t2	dw	0
	t23	dw	0
	t3	dw	0
	t24	dw	0
	t4	dw	0
	t25	dw	0
	t5	dw	0
	t26	dw	0
	t6	dw	0
	t27	dw	0
	t7	dw	0
	t29	dw	0
	t9	dw	0
	t10	dw	0
	t11	dw	0
	t12	dw	0
	t13	dw	0
	t14	dw	0
	t15	dw	0
	t16	dw	0
	t17	dw	0
	t18	dw	0
	t19	dw	0
	t30	dw	0
	t31	dw	0
% ---------------- Allocate function var/param temp data ----------------
	Global_program_str	res	44
	Global_program_n	dw	0
	Global_program_char	dw	0
	Global_program_count	dw	0
	Global_program_i	dw	0
	Global_numToChar_number	dw	0
	Global_numToChar_ascii	dw	0
	Global_numToChar_return	dw	0
	Global_charToNum_char	dw	0
	Global_charToNum_number	dw	0
	Global_charToNum_return	dw	0
% ---------------- Create function definitions ----------------
Global_numToChar	nop
	lw	r9,Global_numToChar_number(r0)	% load number into register
	sub	r1,r1,r1	% Clear register
	addi	r1,r1,0	% load 0 into register
	cge	r9,r9,r1
	bz	r9,else_6my5o9xs
% ---------------- If block ----------------
	lw	r1,Global_numToChar_number(r0)	% load number into register
	sub	r2,r2,r2	% Clear register
	addi	r2,r2,48	% load 48 into register
	add	r1,r1,r2
	sw	Global_numToChar_ascii(r0),r1	% ascii assignment operation
	j	endif_6my5o9xs
% ---------------- else block ----------------
else_6my5o9xs	nop
	sub	r1,r1,r1	% Clear register
	addi	r1,r1,47	% load 47 into register
	sw	Global_numToChar_ascii(r0),r1	% ascii assignment operation
endif_6my5o9xs	nop
% ---------------- end of else block ----------------
	sub	r9,r9,r9	% Clear register
	lw	r9,Global_numToChar_ascii(r0)	% load ascii into register
	sw	Global_numToChar_return(r0),r9	% Store the return value into the return temp memory
	lw	r14,Global_numToChar_return(r0)	% Return the value to the r14 return register
	jr	r15
Global_charToNum	nop
	lw	r9,Global_charToNum_char(r0)	% load char into register
	sub	r1,r1,r1	% Clear register
	addi	r1,r1,48	% load 48 into register
	sub	r9,r9,r1
	sw	Global_charToNum_number(r0),r9	% number assignment operation
	lw	r9,Global_charToNum_number(r0)	% load number into register
	sw	Global_charToNum_return(r0),r9	% Store the return value into the return temp memory
	lw	r14,Global_charToNum_return(r0)	% Return the value to the r14 return register
	jr	r15
% ---------------- Start of user program ----------------
	align
	entry
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,112	% load 112 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,0	% load 0 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,117	% load 117 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,1	% load 1 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,116	% load 116 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,2	% load 2 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,32	% load 32 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,3	% load 3 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,110	% load 110 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,4	% load 4 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,117	% load 117 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,5	% load 5 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,109	% load 109 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,6	% load 6 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,98	% load 98 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,7	% load 7 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,101	% load 101 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,8	% load 8 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,114	% load 114 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,9	% load 9 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,10	% load 10 into register
	sub	r1,r1,r1	% Clear register
	sub	r3,r3,r3	% Clear register
	addi	r3,r3,10	% load 10 into register
	muli	r2,r3,4
	add	r1,r1,r2
	sw	Global_program_str(r1),r9	% str assignment operation
% ---------------- for block ----------------
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,0	% load 0 into register
	sw	Global_program_n(r0),r9	% n assignment operation
gowhile_cy056rl1	nop	% Loop header
	lw	r9,Global_program_n(r0)	% load n into register
	sub	r1,r1,r1	% Clear register
	addi	r1,r1,10	% load 10 into register
	cle	r9,r9,r1
	bz	r9,endwhile_cy056rl1
	sub	r2,r2,r2	% Clear register
	lw	r4,Global_program_n(r0)	% load n into register
	muli	r3,r4,4
	add	r2,r2,r3
	lw	r1,Global_program_str(r2)	% load str[n] into register
	putc	r1	% put the ascii value to the output
	lw	r1,Global_program_n(r0)	% load n into register
	sub	r2,r2,r2	% Clear register
	addi	r2,r2,1	% load 1 into register
	add	r1,r1,r2
	sw	Global_program_n(r0),r1	% n assignment operation
	j	gowhile_cy056rl1
endwhile_cy056rl1	nop
% ---------------- end of for block ----------------
	sub	r9,r9,r9	% Clear register
	getc	r9	% Read the ascii value from the input
	sw	Global_program_char(r0),r9
	sub	r9,r9,r9	% Clear register
	lw	r1,Global_program_char(r0)	% load char into register
	sw	Global_charToNum_char(r0),r1
	jl	r15,Global_charToNum	% Jump to Global_charToNum function
	sw	t28(r0),r14
	lw	r9,t28(r0)
	sw	Global_program_count(r0),r9	% count assignment operation
% ---------------- for block ----------------
	sub	r9,r9,r9	% Clear register
	addi	r9,r9,0	% load 0 into register
	sw	Global_program_i(r0),r9	% i assignment operation
gowhile_k7wbg9hd	nop	% Loop header
	lw	r9,Global_program_i(r0)	% load i into register
	lw	r1,Global_program_count(r0)	% load count into register
	clt	r9,r9,r1
	bz	r9,endwhile_k7wbg9hd
	lw	r2,Global_program_i(r0)	% load i into register
	sw	Global_numToChar_number(r0),r2
	jl	r15,Global_numToChar	% Jump to Global_numToChar function
	sw	t28(r0),r14
	lw	r1,t28(r0)
	putc	r1	% put the ascii value to the output
	lw	r1,Global_program_i(r0)	% load i into register
	sub	r2,r2,r2	% Clear register
	addi	r2,r2,1	% load 1 into register
	add	r1,r1,r2
	sw	Global_program_i(r0),r1	% i assignment operation
	j	gowhile_k7wbg9hd
endwhile_k7wbg9hd	nop
% ---------------- end of for block ----------------
	sub	r9,r9,r9	% Clear register
	hlt
% ---------------- end of program ----------------
