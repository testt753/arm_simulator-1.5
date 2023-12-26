.global main
.text

main:
    mov r0, #5
    ldr r1, =source
    ldrb r2, [r1]
    add r0, r0, r2
    swi 0x123456

.data
source:
    .byte 10