DATA SEGMENT 
DATA ENDS
CODE SEGMENT 
ASSUME CS:CODE , DS:DATA

START : MOV AX,2000H
        MOV DS,AX 
        MOV SI,0500H 
        MOV DI,0600H
        MOV CL,04H 
        MOV AL, [SI]
L2:     MOV CMP AL , [SI+1]
        JB L1
        MOV AL,[SI+1]
L1:     INC SI
        DEC CL 
        JNZ L2 
        MOV [DI],AL 
        INT 21H
CODE ENDS
END START