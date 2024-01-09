.equ MEM_BASE, 0x2000
.global main
.text
.data

main:
    ldr r0, =MEM_BASE
    mov r1, #0x12 
    strb r1, [r0] 

    mov r1, #0x34
    strh r1, [r0, #2] 

    mov r1, #0x0078
    str r1, [r0, #4] 

    
    ldrb r1, [r0] 
    strb r1, [r0, #8] 

    ldrh r1, [r0, #2] 
    strh r1, [r0, #0xA] 

    ldr r1, [r0, #4] 
    str r1, [r0, #0xC] 

    
    ldrb r1, [r0, #8] 
    cmp r1, #0x12 

    swi 0x123456

source_data:
    .byte 0x42
    .byte 0x84
    .short 0x1234
    .short 0xABCD
    .word 0x11223344
    .word 0x55667788