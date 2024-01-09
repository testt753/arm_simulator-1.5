.global main
.text

main:
    ldr r0, =data
    ldrb r1, [r0]
    add r1, r1, #1
    cmp r1, #10
    swi 0x123456

.data
data:
    .byte 9