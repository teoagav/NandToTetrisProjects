@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@0
D=D+A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
($LOOP_START)
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
M=D+M
@SP
M=M-1
@LCL
D=M
@0
D=D+A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
M=M-D
@SP
M=M-1
@ARG
D=M
@0
D=D+A
@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D
@SP
M=M-1
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@$LOOP_START
D;JNE
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
