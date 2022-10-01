//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/22/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "encrypter.h"

    .text     // start of text segment

    //////////////////////////////////////////////////
    // int decrypt(char *IOBUF, char *BOOKBUF, int cnt) 
    // decrypts  IOBUF using BOOKBUF; result in IOBUF
    //////////////////////////////////////////////////
    .global decrypt
    .type   decrypt, %function
    .equ    FP_OFF, 12

decrypt:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF
    mov     r3, 0
.Lloop:
    cmp     r3, r2
    bge     .Ldone
    ldrb    r4, [r1, r3]
    ldrb    r5, [r0, r3]
    eor     r5, r5, r4
    mov     r4, r5
    and     r4, r4, 0x0f
    and     r5, r5, 0xf0
    lsl     r4, r4, 28
    lsr     r4, r4, 24
    lsl     r5, r5, 24
    lsr     r5, r5, 28
    orr     r5, r4, r5
    strb    r5, [r0, r3]
    add     r3, r3, 1
    b       .Lloop
    
.Ldone:
    mov     r0, r3
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size decrypt, (. - decrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
