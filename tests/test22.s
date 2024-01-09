.global main

.section .data
result_and: .space 4
result_eor: .space 4
result_sub: .space 4
result_rsb: .space 4

.section .text
main:
     @ Test de AND avec rotation immédiate
      ldr r9 , =#0x12345678
    mov r0, r9    @ Valeur initiale
    and r1, r0, ror #4    @ Effectue un AND avec une rotation immédiate de 4 bits
     ldr r8, =result_and  
    str r1, [r8]  @ Stocke le résultat dans la mémoire

    @ Test de EOR avec rotation immédiate
    ldr r9 , =result_eor
    ldr r8 , =#0xAABBCCDD
    mov r2, r8  @ Autre valeur initiale
    eor r3, r2, ror #8    @ Effectue un EOR avec une rotation immédiate de 8 bits
    str r3, [r9]  @ Stocke le résultat dans la mémoire

    @ Test de SUB avec rotation immédiate
    mov r4, #100           @ Autre valeur initiale
    sub r5, r4, ror #12    @ Effectue un SUB avec une rotation immédiate de 12 bits
    ldr r11 , =result_sub
    str r5, [r10]   @ Stocke le résultat dans la mémoire

    @ Test de RSB avec rotation immédiate
    ldr r13, =#0x87654321
    mov r6, r13    @Autre valeur initiale
    rsb r7, r6, ror #16    @ Effectue un RSB avec une rotation immédiate de 16 bits
    ldr r12 , =result_rsb
    str r7, [r12]   @ Stocke le résultat dans la mémoire

    swi 0x123456