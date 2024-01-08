.global main
.text

main:
    mov r0, #2
    mov r1, #10   
    sub r4, r0, r1    ; r4 = r0 - r1
    mrs r0, cpsr       ; Copie du registre d'état du programme dans r0

    tst r0, #0x80000000  ; Bit N dans le CPSR (32e bit) flag N
    tst r0, #0x40000000  ; Bit Z dans le CPSR (31e bit) flag Z
    tst r0, #0x20000000  ; Bit C dans le CPSR (30e bit) flag C (Carry)
    tst r0, #0x10000000  ; Bit V dans le CPSR (29e bit) flag V (Overflow)

    mov r5,#7
    msrreg   cpsr, r5

    msrimm   cpsr, #0x10

    swi 0x123456