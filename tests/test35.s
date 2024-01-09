.global main
.text

main:                   @tester mul
    ldr r0, =oper1
    ldr r1, =oper2
    mul r0, r0, r1
end:
    swi 0x123456
.data
    oper1: .word 3
    oper2: .word 2