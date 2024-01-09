.equ MODE_USR, 0x10 @ mode utilisateur 
.equ MODE_FIQ, 0x11 @ mode FIQ
.equ MODE_SVC, 0x13 @ mode SVC
.global main
.text

main:
     MSR CPSR_f,#0xF0000000
    MRS R0,CPSR
    BIC R5,R0,#0x1F
    ORR R5,R5,#MODE_SVC
    cmp r0, r5 @ vérifier mode utilisateur


    BIC R5,R5,#0x1F
    ORR R5,R5,#MODE_FIQ
    msr cpsr_c, R5

    mrs r1, cpsr
    cmp r1, R5

    swi 0x123456