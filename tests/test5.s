.global main
.text

main:
    mov r0, #42
    mov r1, #18
    add r2, r0, r1
    swi 0x123456