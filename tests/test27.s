.global main

.section .data
data_value:  .word 0xAABBCCDD
result_add:   .space 4
result_adc:   .space 4
result_sbc:   .space 4
result_rsc:   .space 4

.section .text
main:
    ldr r0, =data_value

    // Test de ADD avec décalage immédiat
    add r1, r0, #0x1234  // ADD avec décalage immédiat de 0x1234
    str r1, [result_add]

    // Test de ADC avec décalage immédiat
    adc r2, r0, #0x5678  // ADC avec décalage immédiat de 0x5678
    str r2, [result_adc]

    // Test de SBC avec décalage immédiat
    sbc r3, r0, #0x9ABC  // SBC avec décalage immédiat de 0x9ABC
    str r3, [result_sbc]

    // Test de RSC avec décalage immédiat
    rsc r4, r0, #0xDEF0  // RSC avec décalage immédiat de 0xDEF0
    str r4, [result_rsc]

    // Fin du programme
    swi 0x123456
