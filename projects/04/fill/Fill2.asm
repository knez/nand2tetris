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

(LOOP)
	@KBD
	D=M
	@PRESSED	
	D;JGT
	@SCREEN
	D=M
	@INIT
	D;JLT
	@LOOP
	0;JMP
	
(PRESSED)
	@SCREEN
	D=M
	@INIT
	D;JEQ
	@LOOP
	0;JMP	

(INIT)
	@i
	MD=0
	
(PAINT)
	@8191
	D=A-D
	@LOOP
	D;JLT
	@SCREEN
	A=A+D
	M=!M
	@i
	MD=M+1
	@PAINT
	0;JMP
