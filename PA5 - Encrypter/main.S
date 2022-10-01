//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // function import
    .extern encrypt
    .extern decrypt
    .extern setup
    .extern rdbuf
    .extern cleanup
    .extern wrbuf

    // global constants
    #include "encrypter.h"

    .text

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // encrypter [-d | -e] -b bookfile encryption_file  //
    //////////////////////////////////////////////////////

    // finish the stack frame first
    .global main
    .type   main, %function
    .equ    BUFSZ,      1024        // buffer size
    .equ    FP_OFF,     28          // fp offset in main stack frame
    .equ    FPBOOK,     4 + FP_OFF    // FILE * to book file
    .equ    FPIN,       4 + FPBOOK  // FILE * to input file
    .equ    FPOUT,      4 + FPIN     // FILE * to output file
    .equ    MODE,       4 + FPOUT    // decrypt or encrypt mode
    .equ    IOBUF,      BUFSZ + MODE // buffer for input file
    .equ    BOOKBUF,    BUFSZ + IOBUF // buffer for book file
    // add local variables here: adjust PAD size
    .equ    PAD,        0 + BOOKBUF   // Stack frame PAD 
    .equ    OARG6,      4 + PAD       // output arg6
    .equ    OARG5,      4 + OARG6    // output arg5 must be at bottom
    .equ    FRAMESZ,    OARG5-FP_OFF// total space for frame
    ////////////////////////////////////////////////////////////////
    // make sure that FRAMESZ + FP_OFF + 4 divides by 8 EVENLY!
    ////////////////////////////////////////////////////////////////
 
    ////////////////////////////////////////
    // preserved register use             //
    // r4 pointer to input/output buffer  //
    // r5 pointer to book buffer          //
    // r6 mode flag                       //
    // r7 input FILE *                    //
    // r8 book FILE *                     //
    // r9 output FILE *                  //
    ////////////////////////////////////////

main:
    push    {r4-r9, fp, lr}
    add     fp, sp, FP_OFF          // set frame pointer to frame base
    ldr     r3, =FRAMESZ            // frame size is big, use pseudo ldr
    sub     sp, sp, r3              // allocate space for locals and passed args
    sub     r6, fp, MODE
    sub     r7, fp, FPIN
    sub     r8, fp, FPBOOK
    sub     r9, fp, FPOUT
    str     r7, [fp, -OARG5]
    str     r9, [fp, -OARG6]
    mov     r2, r6
    mov     r3, r8
    bl      setup
    ldr     r4, =IOBUF
    sub     r4, fp, r4
    ldr     r5, =BOOKBUF
    sub     r5, fp, r5
    ldr     r6, [r6]
    ldr     r7, [r7]
    ldr     r8, [r8]
    ldr     r9, [r9]

.Lloop:
    ldr     r0, =BUFSZ
    str     r0, [fp, -OARG5]
    mov     r0, r4
    mov     r1, r5
    mov     r2, r7
    mov     r3, r8
    bl      rdbuf
    cmp     r0, 0
    ble     .Ldone

    mov     r2, r0
    mov     r0, r4
    mov     r1, r5
    cmp     r6, ENCRYPT_MODE
    bne     .Ldecrypt
    bl      encrypt
    b       .Lwriteout

.Ldecrypt: 
    bl      decrypt

.Lwriteout:
    ldr     r1, =EXIT_FAIL
    cmp     r0, r1
    beq     .Lexit
    mov     r2, r9
    mov     r1, r0
    mov     r0, r4
    bl      wrbuf
    cmp     r0, EXIT_FAIL
    beq     .Lexit
    b       .Lloop

.Ldone:
    bl      cleanup

.Lexit:
    sub     sp, fp, FP_OFF
    pop     {r4-r9, fp, lr}
    bx      lr

    .size   main, (. - main)
    .section .note.GNU-stack,"",%progbits
.end
#endif
