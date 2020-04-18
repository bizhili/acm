ORG 0000H
LJMP START

start:
    MOV SCON, #0x50;0101 0000usart mode 1 
    ANL PCON, #0x7F;smod=0
    MOV TMOD, #0x20;timer1 works as Baud rate producer
    MOV TH1, #0xFD;SET Baud rete=9600,as core rate@11.0592MHz
    MOV TL1, #0xFD;
    SETB TR1; start timer counter
    MOV (61H),P2 ;STORE P2 TO 61H
LOOP:
    MOV A,(61H)
    MOV (60H),P2;STORE P2 AT 98H
    CJNE A,(60H),SEND ;A AND P2 DIFFER,JUM TO SEND
    LJMP LOOP
SEND:
    MOV A,(60H);UPDATE VALUE
    MOV (61H),A;STORE A
    MOV SBUF, A
    JNB TI, $  ;WAITING FOR SENDING
    CLR TI
    LJMP LOOP

END