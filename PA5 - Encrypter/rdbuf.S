//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/22/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // global constants
    #include "encrypter.h"

    .section .rodata
.Lmsg:  .string "rdbuf: Bookfile is too short for message\n"
.Lmsg2: .string "rdbuf: Buffer size error\n"
    .text

    //////////////////////////////////////////////////////
    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN
    //              FILE *FPBOOK, int BUFSZ);
    // returns number of bytes read EXIT_FAIL otherwise
    //////////////////////////////////////////////////////

    .type   rdbuf, %function
    .global rdbuf
    .equ    IARG5,  4 
    .equ    FP_OFF, 12
 
    ////////////////////////////////////////
    // preserved register use             //
    // r4 pointer to book buffer          //
    // r5 FILE *FPBOOK  & cnt read saved  //
    ////////////////////////////////////////

rdbuf:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF
    mov     r4, r1 
    mov     r5, r3
    ldr     r1, [fp, IARG5]
    cmp     r1, 0
    bgt     .Lif
    // inside if statement
    ldr     r0, =.Lmsg2
    bl      errmsg
    mov     r0, EXIT_FAIL
    b       .Lexit
    // error message & return exitfail

.Lif:
    mov     r3, r2
    ldr     r2, [fp, IARG5]
    mov     r1, 1
    bl      fread
    cmp     r0, 0
    beq     .Lexit
    
//second if
    mov     r3, r5
    mov     r2, r0
    mov     r1, 1
    // set r5 to r0 to compare later since r2 is changed
    mov     r5, r0
    mov     r0, r4
    bl      fread
    cmp     r0, r5
    beq     .Lexit
    // inside if statement
    ldr     r0, =.Lmsg
    bl      errmsg
    mov     r0, EXIT_FAIL

.Lexit:

    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size   rdbuf, (. - rdbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
