.global main

.section .data
data_value:  .word 0xAABBCCDD
result_and:  .space 4
result_eor:  .space 4
result_sub:  .space 4
result_rsb:  .space 4

.section .text
main:
    ldr r0, =data_value

    // Test de AND avec décalage vers la droite de 8 bits
    and r1, r0, r0, lsr #8
    str r1, [result_and]

    // Test de EOR avec décalage vers la gauche de 4 bits
    eor r2, r0, r0, lsl #4
    str r2, [result_eor]

    // Test de SUB avec décalage vers la droite de 16 bits
    sub r3, r0, r0, asr #16
    str r3, [result_sub]

    // Test de RSB avec décalage vers la gauche de 12 bits
    rsb r4, r0, r0, lsl #12
    str r4, [result_rsb]

    // Fin du programme
    swi 0x123456
