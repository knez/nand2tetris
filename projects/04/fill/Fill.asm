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

	@SCREEN
	D=A
	@j
	M=D

(LOOP)
	@KBD
	D=M
	@PRESSED	
	D;JGT
	@SCREEN
	D=M
	@CLEAR
	D;JLT
	@LOOP
	0;JMP
	
(PRESSED)
	@SCREEN
	D=M
	@BLACKEN
	D;JEQ
	@LOOP
	0;JMP	

(BLACKEN)
	@j
	D=M
	@24575
	D=A-D
	@LOOP
	D;JLT
	@j
	A=M
	M=-1
	@j
	M=M+1
	@BLACKEN
	0;JMP

(CLEAR)
	@j
	D=M
	@16384
	D=D-A
	@LOOP
	D;JLT
	@j
	A=M
	M=0
	@j
	M=M-1
	@CLEAR
	0;JMP
