.global main

.section .data
data_a:   .word 0x00010001, 0x00020002, 0x00030003, 0x00040004
data_b:   .word 0xFFFF0001, 0xFFFF0002, 0xFFFF0003, 0xFFFF0004
data_c:   .word 0x00010001, 0x00020002, 0x00030003, 0x00040004
data_d:   .word 0xFFFF0001, 0xFFFF0002, 0xFFFF0003, 0xFFFF0004

.section .text
main:
    // Test de SMLAxy
    ldr r0, =data_a
    ldr r1, =data_b
    smlaxy r2, r3, r0, r1  // Effectue le produit de r0 et r1 avec une addition du résultat, stocke la partie basse dans r2 et la partie haute dans r3

    // Test de SMULWy
    ldr r0, =data_c
    ldr r1, =data_d
    smulwy r2, r3, r0, r1  // Effectue le produit signé des éléments de r0 et r1, stocke la partie basse dans r2 et la partie haute dans r3

    // Test de SMLAWy
    ldr r0, =data_a
    ldr r1, =data_b
    smlawy r2, r3, r0, r1  // Effectue le produit signé de r0 et r1 avec une addition du résultat, stocke la partie basse dans r2 et la partie haute dans r3

    // Test de SMLALxy
    ldr r0, =data_c
    ldr r1, =data_d
    smlalxy r2, r3, r0, r1  // Effectue le produit signé de r0 et r1 avec une addition du résultat aux valeurs existantes dans r2 et r3

    // Test de SMULxy
    ldr r0, =data_a
    ldr r1, =data_b
    smulxy r2, r3, r0, r1  // Effectue le produit signé des éléments de r0 et r1, stocke la partie basse dans r2 et la partie haute dans r3

    // Fin du programme
    swi 0x123456
