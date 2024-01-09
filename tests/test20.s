.global main
.text

main:
     b loop_b        @Boucle avec b (branchement inconditionnel)
     mov r1, #5      @Initialisation du compteur

loop_b:
    add r1, r1, #1  @incrementation du compteur
    cmp r1, #10     @condition de sortie de b
    blt loop_b      @branchement tant que la condition est vraie
    mov r2, #3      @Initialisation du un autre compteur

loop_bl:
    add r2, r2, #1   @incrementation du compteur
    cmp r1, #8       @condition de sortie de bl
    ble loop_bl      @branchement tant que la condition est vraie
    mov r2, #3       @numero de service pour exit
    mov r7, #1       @code de retour
    mov r0, #0       @appel systeme pour exit

    swi 0x123456