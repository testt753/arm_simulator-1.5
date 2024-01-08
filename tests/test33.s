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

    // Test de ORR avec décalage vers la gauche (LSL)
    lsl r1, r0, #4      // Décalage de 4 bits vers la gauche
    orr r1, r1, r0      // Applique l'opération ORR avec la valeur initiale
    str r1, [result_orr]

    // Test de MOV avec décalage vers la droite logique (LSR)
    lsr r2, r0, #8      // Décalage de 8 bits vers la droite logique
    mov r2, r2          // MOV n'affecte pas le résultat du décalage, utilisé ici pour l'exemple
    str r2, [result_mov]

    // Test de BIC avec décalage arithmétique vers la droite (ASR)
    asr r3, r0, #2      // Décalage arithmétique de 2 bits vers la droite
    bic r3, r3, #0xFF   // Applique l'opération BIC avec un masque
    str r3, [result_bic]

    // Test de MVN avec décalage circulaire vers la droite (ROR)
    ror r4, r0, #12     // Décalage circulaire de 12 bits vers la droite
    mvn r4, r4          // Effectue la négation logique (NOT)
    str r4, [result_mvn]

    // Fin du programme
    swi 0x123456
