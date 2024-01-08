.global main
.text

main:                   @tester ldmia
    ldr r0, =adress_mem
    ldmia r0!, {r1, r2, r3, r4, r5}
end:
    swi 0x123456
.data
    adress_mem: .word 0x1000
    