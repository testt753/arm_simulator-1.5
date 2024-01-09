.global main
.text

main:                   @tester umlal
    ldr r0, =oper1
    ldr r2, =oper2
    ldr r4, =acc_low
    ldr r5, =acc_high
    umlal r4, r5, r0, r2
end:
    swi 0x123456
.data
    acc_low: .word 0x00000001
    acc_high: .word 0x00000001
    oper1_low: .word 0xFFFFFFFF
    oper2_low: .word 0x00000003