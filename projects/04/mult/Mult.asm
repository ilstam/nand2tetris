// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@result
M=0 // result = 0

@i
M=0 // i = 0

(LOOP)
	@i
	D=M
	@R0
	D=M-D
	@END
	D;JEQ // if i < MEM[R0] goto END

	@R1
	D=M
	@result
	M=D+M // result += MEM[R1]

	@i
	M=M+1 // i++

	@LOOP
	0;JMP // goto LOOP

(END)
@result
D=M
@R2
M=D // MEM[R2] = result

(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
