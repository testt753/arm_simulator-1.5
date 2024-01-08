.global main
.text

main:                   @tester stmda
    ldr r0, =adress_stoc
    ldr r1, #1
    ldr r2, #2
    ldr r3, #3
    ldr r4, #4
    ldr r5, #5
    stmda r0, {r1, r2, r3, r4, r5}!
end:
    swi 0x123456
.data
    adress_stoc: .word 0x1000
    