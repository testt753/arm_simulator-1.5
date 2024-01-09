.global main
.text

main:
    ldr r0, =value
    ldrb r1, [r0]
    cmp r1, #128
    swi 0x123456

.data
value:
    .byte 200