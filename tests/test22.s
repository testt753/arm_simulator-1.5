.global main

.section .data
result_and: .space 4
result_eor: .space 4
result_sub: .space 4
result_rsb: .space 4

.section .text
main:
    @ Test de AND avec rotation immédiate
    mov r0, #0x12345678   @ Valeur initiale
    and r1, r0, ror #4    @ Effectue un AND avec une rotation immédiate de 4 bits
    str r1, [result_and]  @ Stocke le résultat dans la mémoire

    @ Test de EOR avec rotation immédiate
    mov r2, #0xAABBCCDD   @ Autre valeur initiale
    eor r3, r2, ror #8    @ Effectue un EOR avec une rotation immédiate de 8 bits
    str r3, [result_eor]  @ Stocke le résultat dans la mémoire

    @ Test de SUB avec rotation immédiate
    mov r4, #100           @ Autre valeur initiale
    sub r5, r4, ror #12    @ Effectue un SUB avec une rotation immédiate de 12 bits
    str r5, [result_sub]   @ Stocke le résultat dans la mémoire

    @ Test de RSB avec rotation immédiate
    mov r6, #0x87654321   @ Autre valeur initiale
    rsb r7, r6, ror #16    @ Effectue un RSB avec une rotation immédiate de 16 bits
    str r7, [result_rsb]   @ Stocke le résultat dans la mémoire

    @ Fin du programme
    swi 0x123456
