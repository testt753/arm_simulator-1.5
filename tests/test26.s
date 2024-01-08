.global main

.section .data
data_value:      .word 0xAABBCCDD
result_orr:      .space 4
result_mov:      .space 4
result_bic:      .space 4
result_mvn:      .space 4

.section .text
main:
    ldr r0, =data_value

    // Test de ORR avec rotation immédiate
    mov r1, r0, ror #8   // Rotation immédiate de 8 bits
    orr r2, r0, r1       // ORR avec la valeur initiale et la rotation
    str r2, [result_orr]

    // Test de MOV avec rotation immédiate
    mov r3, r0, ror #8   // Rotation immédiate de 8 bits
    str r3, [result_mov]

    // Test de BIC avec rotation immédiate
    mov r4, r0, ror #8   // Rotation immédiate de 8 bits
    bic r5, r0, r4       // BIC avec la valeur initiale et la rotation
    str r5, [result_bic]

    // Test de MVN avec rotation immédiate
    mov r6, r0, ror #8   // Rotation immédiate de 8 bits
    mvn r7, r6           // MVN avec la rotation
    str r7, [result_mvn]

    // Fin du programme
    swi 0x123456
