.global main

.section .data
data_value:  .word 0xAABBCCDD
result_orr:   .space 4
result_mov:   .space 4
result_bic:   .space 4
result_mvn:   .space 4

.section .text
main:
    ldr r0, =data_value

    // Test de ORR avec décalage immédiat
    orr r1, r0, r0, lsl #8   // Effectue un OR logique avec la valeur décalée immédiatement
    str r1, [result_orr]

    // Test de MOV avec décalage immédiat
    mov r2, r0, lsr #16      // Effectue un décalage logique vers la droite immédiatement
    str r2, [result_mov]

    // Test de BIC avec décalage immédiat
    bic r3, r0, r0, asr #4   // Effectue un AND avec le complément à deux de la valeur décalée immédiatement
    str r3, [result_bic]

    // Test de MVN avec décalage immédiat
    mvn r4, r0, lsl #12      // Effectue un NOT logique sur la valeur décalée immédiatement
    str r4, [result_mvn]

    // Fin du programme
    swi 0x123456
