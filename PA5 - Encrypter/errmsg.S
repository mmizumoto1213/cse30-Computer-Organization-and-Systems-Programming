//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.01 05/27/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    // imports
    .extern stderr
    .extern fprintf

    .text

    ////////////////////////////////////////////
    // void errmsg(char *errormsg)            //
    // writes error messages to stderr        //
    ////////////////////////////////////////////
    .type   errmsg, %function
    .global errmsg
    .equ    FP_OFF,  4

errmsg:
    push    {fp, lr}
    add     fp, sp, FP_OFF
    mov     r1, r0
    ldr     r0, =stderr
    ldr     r0, [r0]
    bl      fprintf
    sub     sp, fp, FP_OFF
    pop     {fp, lr}
    bx      lr

    .size errmsg, (. - errmsg)
    .section .note.GNU-stack,"",%progbits
.end
#endif
