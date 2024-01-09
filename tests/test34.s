.global main

.section .data
data_value:  .word 0xAABBCCDD
data_array:  .space 16
result_str:  .space 4
result_ldr:  .space 4
result_strb: .space 4
result_ldrb: .space 4

.section .text
main:
    ldr r0, =data_value

    // Test de STR avec un registre
    str r0, [data_array]  // Stocke la valeur de r0 dans le tableau data_array
    ldr r1, [data_array]  // Charge la valeur stockée dans data_array dans r1
    str r1, [result_str]  // Stocke le résultat dans la mémoire

    // Test de STR avec une valeur immédiate
    str r0, [data_array, #4]  // Stocke la valeur de r0 dans le tableau data_array avec un décalage de 4 octets
    ldr r2, [data_array, #4]  // Charge la valeur stockée dans data_array avec un décalage de 4 octets dans r2
    str r2, [result_str + #4]  // Stocke le résultat dans la mémoire avec un décalage de 4 octets

    // Test de LDR avec un registre
    ldr r3, [data_array, #8]  // Charge la valeur stockée dans data_array avec un décalage de 8 octets dans r3
    str r3, [result_ldr]      // Stocke le résultat dans la mémoire

    // Test de LDR avec une valeur immédiate
    ldr r4, [data_array, #12]  // Charge la valeur stockée dans data_array avec un décalage de 12 octets dans r4
    str r4, [result_ldr + #4]  // Stocke le résultat dans la mémoire avec un décalage de 4 octets

    // Test de STRB avec un registre
    mov r5, #0xFF       // Valeur à stocker dans le byte de poids faible
    strb r5, [result_strb]  // Stocke la valeur de r5 (byte de poids faible) dans la mémoire

    // Test de STRB avec une valeur immédiate
    strb r5, [result_strb + #1]  // Stocke la valeur de r5 (byte de poids faible) dans la mémoire avec un décalage de 1 octet

    // Test de LDRB avec un registre
    ldrb r6, [result_strb]  // Charge le byte de poids faible stocké dans la mémoire dans r6
    str r6, [result_ldrb]   // Stocke le résultat dans la mémoire

    // Test de LDRB avec une valeur immédiate
    ldrb r7, [result_strb + #1]  // Charge le byte de poids faible stocké dans la mémoire avec un décalage de 1 octet dans r7
    str r7, [result_ldrb + #4]   // Stocke le résultat dans la mémoire avec un décalage de 4 octets

    // Fin du programme
    swi 0x123456
