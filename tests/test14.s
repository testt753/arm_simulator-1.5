.global main
.text

main:
    ldr r0, =array
    ldmia r0!, {r1, r2, r3}
    sub r4, r1, r2
    add r5, r4, r3
    swi 0x123456

.data
array:
    .word 10, 5, 7