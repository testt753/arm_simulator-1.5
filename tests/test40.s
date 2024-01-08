.global main
.text

main:                   @tester smull
    ldr r0, =oper1
    ldr r1, =oper2
    ldr r2, =acc_low
    ldr r3, =acc_high
    smlal r2, r3, r0, r1
end:
    swi 0x123456
.data
    acc_low: .word 1
    acc_high: .word 1
    oper1: .word -5
    oper2: .word 2