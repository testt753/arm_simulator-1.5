.global main
.text

main:
    mov r0, #2
    mov r1, #10   
    sub r4, r0, r1    
    mrs r0, cpsr      

    mov r5,#7
    msr   cpsr, r5

    msr   cpsr, #0x10

    swi 0x123456