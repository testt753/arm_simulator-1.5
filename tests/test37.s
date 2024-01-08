.global main
.text

main:                   @tester umull
    ldr r0, =oper1
    ldr r1, =oper2
    ldr r2, #0
    ldr r3, #0
    umull r2, r3, r0, r1
end:
    swi 0x123456
.data
    acc: .word 1
    oper1: .word 0xFFFFFFFF
    oper2: .word 0x00000003