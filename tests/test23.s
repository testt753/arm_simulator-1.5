.global main

.section .data
result_and: .space 4
result_eor: .space 4
result_sub: .space 4
result_rsb: .space 4

.section .text
main:
    // Test de AND avec décalage immédiat
    mov r0, #0x12345678   // Valeur initiale
    and r1, r0, lsl #4    // Effectue un AND avec un décalage immédiat de 4 bits à gauche
    str r1, [result_and]  // Stocke le résultat dans la mémoire

    // Test de EOR avec décalage immédiat
    mov r2, #0xAABBCCDD   // Autre valeur initiale
    eor r3, r2, lsr #8    // Effectue un EOR avec un décalage immédiat de 8 bits à droite
    str r3, [result_eor]  // Stocke le résultat dans la mémoire

    // Test de SUB avec décalage immédiat
    mov r4, #100           // Autre valeur initiale
    sub r5, r4, asr #12   // Effectue un SUB avec un décalage arithmétique immédiat de 12 bits à droite
    str r5, [result_sub]   // Stocke le résultat dans la mémoire

    // Test de RSB avec décalage immédiat
    mov r6, #0x87654321   // Autre valeur initiale
    rsb r7, r6, ror #16   // Effectue un RSB avec un décalage immédiat de 16 bits à droite
    str r7, [result_rsb]   // Stocke le résultat dans la mémoire

    // Fin du programme
    swi 0x123456
