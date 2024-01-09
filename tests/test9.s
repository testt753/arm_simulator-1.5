.global main
.text

main:
    ldr r0, =source
    ldrb r1, [r0]
    mov r2, #5
    add r1, r1, r2
    strb r1, [r0]
    swi 0x123456

.data
source:
    .byte 15