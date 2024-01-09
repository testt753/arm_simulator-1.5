.global main
.text

main:
    ldr r0, =data
    ldrh r1, [r0]
    add r1, r1, #1
    strh r1, [r0]
    swi 0x123456

.data
data:
    .short 32767