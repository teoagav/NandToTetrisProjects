@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
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
D=M-D
@EQ0
D;JEQ
@0
D=A
@endEQ0
0;JMP
(EQ0)
@0
D=!A
(endEQ0)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@16
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
D=M-D
@EQ1
D;JEQ
@0
D=A
@endEQ1
0;JMP
(EQ1)
@0
D=!A
(endEQ1)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
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
D=M-D
@EQ2
D;JEQ
@0
D=A
@endEQ2
0;JMP
(EQ2)
@0
D=!A
(endEQ2)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
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
D=M-D
@LT0
D;JLT
@0
D=A
@endLT0
0;JMP
(LT0)
@0
D=!A
(endLT0)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@892
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
D=M-D
@LT1
D;JLT
@0
D=A
@endLT1
0;JMP
(LT1)
@0
D=!A
(endLT1)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
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
D=M-D
@LT2
D;JLT
@0
D=A
@endLT2
0;JMP
(LT2)
@0
D=!A
(endLT2)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
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
D=M-D
@GT0
D;JGT
@0
D=A
@endGT0
0;JMP
(GT0)
@0
D=!A
(endGT0)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32767
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
D=M-D
@GT1
D;JGT
@0
D=A
@endGT1
0;JMP
(GT1)
@0
D=!A
(endGT1)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
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
D=M-D
@GT2
D;JGT
@0
D=A
@endGT2
0;JMP
(GT2)
@0
D=!A
(endGT2)
@SP
A=M
A=A-1
A=A-1
M=D
@SP
M=M-1
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
@53
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
M=D+M
@SP
M=M-1
@112
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
@SP
A=M
A=A-1
M=-M
@SP
A=M
A=A-1
D=M
A=A-1
M=D&M
@SP
M=M-1
@82
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
M=D|M
@SP
M=M-1
@SP
A=M
A=A-1
M=!M
