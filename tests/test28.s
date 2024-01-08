.global main

.section .data
data_value:  .word 0xAABBCCDD

.section .text
main:
    ldr r0, =data_value

    // Test de TST avec décalage immédiat
    tst r1, r0, lsl #8   // Effectue un décalage logique à gauche de 8 bits
                         // et applique l'opération logique TST avec la valeur initiale

    // Test de TEQ avec décalage immédiat
    teq r2, r0, lsr #4   // Effectue un décalage logique à droite de 4 bits
                         // et applique l'opération logique TEQ avec la valeur initiale

    // Test de CMP avec décalage immédiat
    cmp r3, r0, asr #2   // Effectue un décalage arithmétique à droite de 2 bits
                         // et applique l'opération logique CMP avec la valeur initiale

    // Test de CMN avec décalage immédiat
    cmn r4, r0, ror #12  // Effectue une rotation à droite de 12 bits
                         // et applique l'opération logique CMN avec la valeur initiale

    // Fin du programme
    swi 0x123456
