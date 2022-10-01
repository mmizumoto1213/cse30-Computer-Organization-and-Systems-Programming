    .arch armv6             // armv6 architecture
    .arm                    // arm 32-bit instruction set
    .fpu vfp                // floating point co-processor
    .syntax unified         // modern sytax

    //  include files
    #include    "cw.h"

    // name imports
    .extern     setup
    .extern     rd
    .extern     result
    
    // name exports
    .global     buffer
    .global     cntchar
    .global     cntword
    .global     cntline

    .data       // segment
    // allocate space for (1) buffer, (2) cntchar (3) cntword
    // (4) cntline; make sure to watch alignments!
cntchar:.word 0
cntword:.word 0
cntline:.word 0
buffer: .space BUFSZ
    .text       // segment

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // program usage: cw [file]                         //
    //                                                  //
    // you can use any scratch register r0-r3           //
    // you can use these preserved registers            //
    // r4                                               //
    // r5                                               //
    // r6                                               //
    // r7                                               //
    //////////////////////////////////////////////////////

    .global main                // global for linking to
    .type   main, %function     // define as a function
    .equ    EXIT_SUCCESS, 0
    .equ    EXIT_FAILURE, 1
    .equ    FP_OFF, 20          // fp offset in stack frame

main:
    // function prologue
    // do not edit
    push    {r4-r7,fp, lr}      // save fp and lr on stack
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    bl      setup               // call setup(argc, argv)
    cmp     r0, 0               // returns 0 if ok, -1 otherwise
    beq     .Louterloop
    mov     r0, EXIT_FAILURE
    b       .Lexit

/*
 * in a loop call rd() until end of file
 * add your code to
 * (1)  count the number of characters in the words
 * (not including any spaces, newlines or tabs,
 * (2) the number of words. words are delimited by space, \n or \t
 * (3) the number of \n in the input
 */

.Louterloop:

    ldr r5, =cntchar
    ldr r5, [r5]
    ldr r6, =cntword
    ldr r6, [r6]
    ldr r7, =cntline
    ldr r7, [r7]
// here is how to use rd()
    bl rd                  // read another buffer
    cmp r0, 0               // if r0 <= 0 then EOF
    ble .Loutside
    ldr	r4, =buffer
    //ldr     r2, [r2]
    add r3, r4, BUFSZ
    mov r1, 0
// your code here

.Linnerloop:
    cmp r4, r3
    bgt .Louterloop
    ldrb r0, [r4]
    cmp r0, '\n'
    beq .Lnewline // branches to if
    cmp r0, '\t'
    beq .Ltabspace // branches to elif
    cmp r0, ' '
    beq .Ltabspace // branches to elseif
    // else
    cmp r0, 0
    beq .Loutside
    add r5, r5, 1
    b .Lendif

.Lnewline:
    add r7, r7, 1 //line++
    b .Ltabspace
/*
    cmp r1, '\n'
    beq .Lendif
    cmp r1, '\t'
    beq .Lendif
    cmp r1, ' '
    beq .Lendif
    add r6, r6, 1 //word++
    b .Lendif
*/
.Ltabspace:

    cmp r1, '\n'
    beq .Lendif
    cmp r1, '\t'
    beq .Lendif
    cmp r1, ' '
    beq .Lendif
    cmp r5, 0
    beq .Lendif
    add r6, r6, 1 //word++
    b .Lendif

.Lendif:
    ldrb r1, [r4] //r1 holds previous
    add r4, r4, 1 //buffer++
    b .Linnerloop

.Loutside:
    ldr r1, =cntchar
    str r5, [r1]
    ldr r2, =cntword
    str r6, [r2]
    ldr r3, =cntline
    str r7, [r3]
    // do not edit from here to end of this file
    bl      result              // print the results
    mov     r0, EXIT_SUCCESS    // fall through to .Lexit

.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4-r7,fp,lr}       // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size   main, (. - main)    // set size for function
.end
