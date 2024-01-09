.global main

.section .data
data_byte:   .byte  0x12
data_half:   .short 0x1234
data_word:   .word  0x12345678
result_byte: .space 1
result_half: .space 2
result_word: .space 4

.section .text
main:
    // Test de LDRSB registre
    ldr r0, =data_byte
    ldrsb r1, [r0]    // Charge le byte signé à l'adresse pointée par r0 dans r1
    str r1, [result_byte]  // Stocke la valeur chargée dans r1 à l'adresse pointée par result_byte

    // Test de LDRSH registre
    ldr r2, =data_half
    ldrsh r3, [r2]    // Charge le halfword signé à l'adresse pointée par r2 dans r3
    strh r3, [result_half]  // Stocke la valeur chargée dans r3 à l'adresse pointée par result_half

    // Test de LDRSB immédiat
    ldr r4, =data_byte
    ldrsb r5, [r4, #0]  // Charge le byte signé à l'adresse pointée par r4 dans r5
    str r5, [result_byte]  // Stocke la valeur chargée dans r5 à l'adresse pointée par result_byte

    // Test de LDRSH immédiat
    ldr r6, =data_half
    ldrsh r7, [r6, #0]  // Charge le halfword signé à l'adresse pointée par r6 dans r7
    strh r7, [result_half]  // Stocke la valeur chargée dans r7 à l'adresse pointée par result_half

    // Test de STRD registre
    ldr r8, =data_word
    ldr r9, =0xAABBCCDD11223344
    strd r9, [r8]     // Stocke la valeur du doubleword r9 à l'adresse pointée par r8

    // Test de LDRD registre
    ldr r10, =result_word
    ldrd r11, [r8]    // Charge le doubleword à l'adresse pointée par r8 dans r11
    strd r11, [r10]   // Stocke la valeur chargée dans r11 à l'adresse pointée par r10

    // Test de STRD immédiat
    ldr r12, =data_word
    ldrd r13, =0xAABBCCDD11223344
    strd r13, [r12, #0]  // Stocke la valeur du doubleword r13 à l'adresse pointée par r12

    // Test de LDRD immédiat
    ldr r14, =result_word
    ldrd r15, [r12, #0]  // Charge le doubleword à l'adresse pointée par r12 dans r15
    strd r15, [r14]      // Stocke la valeur chargée dans r15 à l'adresse pointée par r14

    // Fin du programme
    swi 0x123456
