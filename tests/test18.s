.global main

.section .data
data_imm:  .short 0x1234
data_reg:  .short 0x5678
result_imm: .space 2
result_reg: .space 2

.section .text
main:
    // Test de STRH immédiat
    ldr r0, =data_imm
    strh 0xABCD, [r0]  // Stocke la valeur immédiate 0xABCD à l'adresse pointée par r0 (data_imm)

    // Test de LDRH immédiat
    ldr r1, =result_imm
    ldrh r2, [r0]      // Charge le demi-mot (16 bits) à l'adresse pointée par r0 dans r2
    strh r2, [r1]      // Stocke la valeur chargée dans r2 à l'adresse pointée par r1 (result_imm)

    // Test de STRH registre
    ldr r3, =data_reg
    ldr r4, =0xABCD
    strh r4, [r3]      // Stocke la valeur du registre r4 à l'adresse pointée par r3 (data_reg)

    // Test de LDRH registre
    ldr r5, =result_reg
    ldrh r6, [r3]      // Charge le demi-mot (16 bits) à l'adresse pointée par r3 dans r6
    strh r6, [r5]      // Stocke la valeur chargée dans r6 à l'adresse pointée par r5 (result_reg)

    // Fin du programme
    swi 0x123456
