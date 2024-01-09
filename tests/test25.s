.global main

.section .data
data_value:  .word 0xAABBCCDD
result_tst:   .space 4
result_teq:   .space 4
result_cmp:   .space 4
result_cmn:   .space 4

.section .text
main:
    ldr r0, =data_value

    // Test de TST avec rotation immédiate
    mov r1, r0, ror #8   // Rotation immédiate de 8 bits
    tst r0, r1           // TST avec la valeur initiale et la rotation
    str r0, [result_tst]

    // Test de TEQ avec rotation immédiate
    mov r2, r0, ror #8   // Rotation immédiate de 8 bits
    teq r0, r2           // TEQ avec la valeur initiale et la rotation
    str r0, [result_teq]

    // Test de CMP avec rotation immédiate
    mov r3, r0, ror #8   // Rotation immédiate de 8 bits
    cmp r0, r3           // CMP avec la valeur initiale et la rotation
    str r0, [result_cmp]

    // Test de CMN avec rotation immédiate
    mov r4, r0, ror #8   // Rotation immédiate de 8 bits
    cmn r0, r4           // CMN avec la valeur initiale et la rotation
    str r0, [result_cmn]

    // Fin du programme
    swi 0x123456
