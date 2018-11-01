// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed.
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

// initialize
@R0
M=0
@R1
M=0

// Jump to place where r0 is set
(loop)
@KBD
D=M
@setTo1
D;JNE

//Set R0 based on if key is pressed or not
@R0
M=0 // r0 = 0
@fill
0;JMP

(setTo1)
@R0
M=-1 // r0 = -1

//Set portion of the screen to R0
(fill)
@SCREEN
D=A // d = screen
@R1
D=D+M // d = screen + r1
@R2
M=D // r2 = screen + r1
@R0
D=M // d = r0

@R2
A=M // a = screen + r1 (r2)
M=D // screen + r1 = r0

@R1
MD=M+1
@8192
D=D-A
@loop
D;JNE
@R1
M=0
@loop
0;JMP
