(SimpleFunction.test)
@SP
AM=M+1
A=A-1
M=0
@SP
AM=M+1
A=A-1
M=0
@0
D=A
@LCL
A=M+D
D=M
@SP
AM=M+1
A=A-1
M=D
@1
D=A
@LCL
A=M+D
D=M
@SP
AM=M+1
A=A-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M+D
@SP
A=M-1
M=!M
@0
D=A
@ARG
A=M+D
D=M
@SP
AM=M+1
A=A-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M+D
@1
D=A
@ARG
A=M+D
D=M
@SP
AM=M+1
A=A-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@5
D=A
@LCL
A=M-D
D=M
@RET
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
D=A+1
@SP
M=D
@LCL
D=M
@TMP
AM=D-1
D=M
@THAT
M=D
@TMP
AM=M-1
D=M
@THIS
M=D
@TMP
AM=M-1
D=M
@ARG
M=D
@TMP
AM=M-1
D=M
@LCL
M=D
@RET
A=M
0;JMP
