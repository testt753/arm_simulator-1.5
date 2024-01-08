.global main
.text

main:                   @tester smull
    ldr r0, =oper1
    ldr r1, =oper2
    ldr r2, #0
    ldr r3, #0
    smull r2, r3, r0, r1
end:
    swi 0x123456
.data
    oper1: .word -5
    oper2: .word 2