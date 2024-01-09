.global main

.section .data
data_value:  .word 0xAABBCCDD
result_swp:  .space 4
result_swpb: .space 4

.section .text
main:
    ldr r1, =data_value
    ldr r2, =result_swp

    ldr r10, =0xEEFF0011    @ Charge la constante dans r10
    mov r3, r10
    swp r4, r3, [r1]        @ Effectue l'échange

    ldr r5, =data_value
    ldr r6, =result_swpb

    ldr r9, =0x22334455     @ Charge la constante dans r9
    mov r7, r9
    swpb r8, r7, [r5]       @ Effectue l'échange de byte
    swi 0x123456