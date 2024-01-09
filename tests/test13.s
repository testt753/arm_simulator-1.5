.global main
.text

main:
    ldr r0, =source
    ldrb r1, [r0]
    cmp r1, #0
    bne not_zero
    mov r2, #1
    b end
not_zero:
    mov r2, #0
    swi 0x123456

.data
source:
    .byte 7