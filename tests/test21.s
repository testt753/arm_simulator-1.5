.global main

.section .data
data_value:  .word 0xAABBCCDD
result_swp:  .space 4
result_swpb: .space 4

.section .text
main:

    // Test de SWP
    ldr r1, =data_value
    ldr r2, =result_swp
    mov r3, #0xEEFF0011
    swp r4, r3, [r1]  // Échange la valeur de r3 avec celle à l'adresse pointée par r1, stocke l'ancienne valeur à l'adresse pointée par r2

    // Test de SWPB
    ldr r5, =data_value
    ldr r6, =result_swpb
    mov r7, #0x22334455
    swpb r8, r7, [r5]  // Échange le byte de poids faible de r7 avec celui à l'adresse pointée par r5, stocke l'ancien byte à l'adresse pointée par r6

    // Fin du programme
   swi 0x123456
