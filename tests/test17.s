.global main


.section .text
main:
   
    ldr r0, =data_a
    ldr r1, =data_b
   
    mul r2, r0, r1      @ Multiplication signée 32 bits : r2 = r0 * r1

    ldr r0, =data_c
    ldr r1, =data_d

    smull r2, r3, r0, r1   @ Multiplication signée 16 bits : r3:r2 = r0 * r1 (stockage sur 64 bits)

    ldr r0, =data_a
    ldr r1, =data_b

    umull r2, r3, r0, r1   @ Multiplication non signée 16 bits : r3:r2 = r0 * r1 (stockage sur 64 bits)

    swi 0x123456

.section .data
data_a:   .word 0x00010001
data_b:   .word 0xFFFF0001
data_c:   .word 0x00010003
data_d:   .word 0x00010002