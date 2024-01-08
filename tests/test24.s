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

    // Test de ADD avec rotation immédiate
    mov r1, r0, ror #8   // Rotation immédiate de 8 bits
    add r2, r0, r1       // ADD avec la valeur initiale et la rotation
    str r2, [result_add]

    // Test de ADC avec rotation immédiate
    mov r3, r0, ror #8   // Rotation immédiate de 8 bits
    adc r4, r0, r3       // ADC avec la valeur initiale et la rotation
    str r4, [result_adc]

    // Test de SBC avec rotation immédiate
    mov r5, r0, ror #8   // Rotation immédiate de 8 bits
    sbc r6, r0, r5       // SBC avec la valeur initiale et la rotation
    str r6, [result_sbc]

    // Test de RSC avec rotation immédiate
    mov r7, r0, ror #8   // Rotation immédiate de 8 bits
    rsc r8, r0, r7       // RSC avec la valeur initiale et la rotation
    str r8, [result_rsc]

    // Fin du programme
    swi 0x123456
