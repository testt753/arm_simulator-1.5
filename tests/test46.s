.global main
.text

main:                   @tester ldmdb
    ldr r0, =adress_mem
    ldmdb r0, {r1, r2, r3, r4, r5}!
end:
    swi 0x123456
.data
    adress_mem: .word 0x1000
    