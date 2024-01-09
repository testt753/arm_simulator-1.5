.global main

.section .data
data_qadd:  .word 0x7FFFFFFF, 0x80000001, 0x7FFFFFFF, 0x80000001
data_qsub:  .word 0x7FFFFFFF, 0x80000001, 0x80000001, 0x7FFFFFFF
data_qdadd: .word 0x7FFFFFFF, 0x80000001, 0x7FFFFFFF, 0x80000001, 0xFFFFFFFF, 0x12345678, 0x87654321, 0x12345678
data_qdsub: .word 0x7FFFFFFF, 0x80000001, 0x80000001, 0x7FFFFFFF, 0xFFFFFFFF, 0x12345678, 0x87654321, 0x12345678

.section .text
main:
    
    ldr q0, =data_qadd
    ldr q1, =data_qadd + 16
    qadd q2, q0, q1  

    
    ldr q3, =data_qsub
    ldr q4, =data_qsub + 16
    qsub q5, q3, q4  

    
    ldr q6, =data_qdadd
    ldr q7, =data_qdadd + 16
    qdadd q8, q6, q7  

    
    ldr q9, =data_qdsub
    ldr q10, =data_qdsub + 16
    qdsub q11, q9, q10  

    swi 0x123456
