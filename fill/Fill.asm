

// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// Adapted to JCE course, 2023b edition
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

@check
0;JMP
(GOSC)
   D=0
   @SCREEN
   M=0
   @LOOP2
   0;JMP
(GOBL)
  
  D=0
  @SCREEN
  M=-1
  D=0
  @LOOP1
  0;JMP
(LOOP1)

  @i
  M=D
  @KBD
  D=M
  @GOSC
  D;JEQ
  @8191
  D=A
  @i
  D=M-D
  @check2
  D;JEQ
  @i
  D=M
  D=D+1
  @SCREEN
  A=A+D
  M=-1
  @LOOP1
  0;JMP
 
 
 
   
(LOOP2)
  @i
  M=D
  @KBD
  D=M
  @GOBL
  D;JGT
  @8191
  D=A
  @i
  D=M-D
  @check1
  D;JEQ
  @i
  D=M
  D=D+1
  @SCREEN
  A=A+D
  M=0
  @LOOP2
  0;JMP
 
(check)
   @KBD
   D=M
   @GOBL
   D;JGT
   @GOSC
   0;JMP
(check1)
   @KBD
   D=M
   @GOBL
   D;JGT
   @check1
   0;JMP
(check2)
   @KBD
   D=M
   @GOSC
   D;JEQ
   @check2
   0;JMP

