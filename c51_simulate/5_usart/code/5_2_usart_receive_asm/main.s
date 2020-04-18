ORG 0000H
LJMP START
org 0023H;receive int add
LJMP RECEIVE
start:
    MOV SCON, #0x50;0101 0000usart mode 1 
    ANL PCON, #0x7F;smod=0
    MOV TMOD, #0x20;timer1 works as Baud rate producer
    MOV TH1, #0xFD;Baud rete=9600,as core rate@11.0592MHz
    MOV TL1, #0xFD;
    MOV IE,#10010000B;enable all interrupt and usart interrupt
    SETB TR1; start timer counter
    SJMP $   ;wait
RECEIVE:
    JB RI,HANDL ;RI==1,jmp to handel to change p1
    RETI
HANDL:
    MOV P1,SBUF
    CLR RI     ;clear flag
    RETI
END