.global main

.section .data
data_value1: .word 0xAABBCCDD
data_value2: .word 0x11223344
result_add:  .space 4
result_adc:  .space 4
result_sbc:  .space 4
result_rsc:  .space 4

.section .text
main:
    ldr r0, =data_value1
    ldr r1, =data_value2

    // Test de ADD avec décalage gauche
    lsl r2, r0, #4   // Décalage gauche de 4 bits
    add r3, r2, r1   // Addition des résultats des décalages
    str r3, [result_add]

    // Test de ADC avec décalage gauche
    lsl r4, r0, #4   // Décalage gauche de 4 bits
    adc r5, r4, r1   // Addition avec retenue des résultats des décalages
    str r5, [result_adc]

    // Test de SBC avec décalage gauche
    lsl r6, r0, #4   // Décalage gauche de 4 bits
    sbc r7, r6, r1   // Soustraction avec retenue des résultats des décalages
    str r7, [result_sbc]

    // Test de RSC avec décalage gauche
    lsl r8, r0, #4   // Décalage gauche de 4 bits
    rsc r9, r8, r1   // Soustraction avec retenue (inverse) des résultats des décalages
    str r9, [result_rsc]

    // Fin du programme
    swi 0x123456
