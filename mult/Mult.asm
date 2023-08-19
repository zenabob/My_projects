// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// Adapted to JCE course, 2023b edition
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.


@R0

D=M
@FIX
D;JLE
@R1
D=M
@FIX 
D;JLE
@R2
M=0

(LOOP)
   @R1 
   D=M
   @check
   D;JEQ
   @R0
   D=M
   @R2
   M=M+D
   @R1
   M=M-1
   @LOOP
   0;JMP
(FIX)
   @R2
   M=0
   @END
   0;JMP
(check)
   @R2
   D=M
   @32767
   D=D-A
   @FIX
   D-1;JGE
(END)
   @END
   0;JMP