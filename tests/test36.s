.global main
.text

main:                   @tester mla
    ldr r0, =oper1
    ldr r1, =oper2
    ldr r2, =acc
    mla r0, r0, r1, r2
end:
    swi 0x123456
.data
    acc: .word 1
    oper1: .word 3
    oper2: .word 2