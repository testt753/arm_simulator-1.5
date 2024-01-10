.global main

main:
    @ Mul
    mov r1, #5
    mov r2, #10
    mul r3, r1, r2   @ r3 = 5 * 10 = 50

    @ Mla
    mov r4, #15
    mla r5, r1, r2, r4   @ r5 = (5 * 10) + 15 = 65

    @ Umull
    mov r1, #20
    mov r2, #30
    umull r3, r4, r1, r2   @ r3 = lower 32 bits of (20 * 30) = 600, r4 = upper 32 bits of (20 * 30) = 0

    @ Umlal
    mov r1, #40
    mov r2, #50
    umlal r3, r4, r1, r2   @ r3 = lower 32 bits of (40 * 50) + r3 = 2000 + r3, r4 = upper 32 bits of (40 * 50) = 0

    @ Smull
    mov r1, #60
    mov r2, #70
    smull r3, r4, r1, r2   @ r3 = lower 32 bits of (60 * 70) = 4200, r4 = upper 32 bits of (60 * 70) = -1 (sign-extended)

    @ Smlal
    mov r1, #80        
    mov r2, #90
    smlal r3, r4, r1, r2   @ r3 = lower 32 bits of (80 * 90) + r3 = 7200 + r3, r4 = upper 32 bits of (80 * 90) = -1 (sign-extended)

    swi 0x123456