.global main
.text

main:
    ldr r0, =data
    ldrh r1, [r0]
    lsl r1, r1, #2
    strh r1, [r0]
    swi 0x123456

.data
data:
    .short 5