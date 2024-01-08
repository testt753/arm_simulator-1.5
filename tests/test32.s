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

    // Test de TST avec décalage
    lsl r1, r0, #8   // Effectue un décalage logique vers la gauche de 8 bits
    tst r0, r1       // Effectue un test logique entre la valeur initiale et le résultat du décalage
    str r1, [result_tst]

    // Test de TEQ avec décalage
    lsl r2, r0, #8   // Effectue un décalage logique vers la gauche de 8 bits
    teq r0, r2       // Effectue un test d'égalité entre la valeur initiale et le résultat du décalage
    str r2, [result_teq]

    // Test de CMP avec décalage
    lsl r3, r0, #8   // Effectue un décalage logique vers la gauche de 8 bits
    cmp r0, r3       // Effectue une comparaison entre la valeur initiale et le résultat du décalage
    str r3, [result_cmp]

    // Test de CMN avec décalage
    lsl r4, r0, #8   // Effectue un décalage logique vers la gauche de 8 bits
    cmn r0, r4       // Effectue une comparaison négative entre la valeur initiale et le résultat du décalage
    str r4, [result_cmn]

    // Fin du programme
    swi 0x123456
