.global main

.equ MEM_BASE, 0x2000 @ base adresse mémoire de test
.section .data
data_imm:  .short 0x1234
data_reg:  .short 0x5678
result_imm: .space 2
result_reg: .space 2

.section .text
main:
   
   ldr r0, =MEM_BASE
    ldr r7, =data_imm
    strh r7, [r0]

   
    ldr r1, =result_imm
    ldrh r2, [r0]      
    strh r2, [r1]    

   
    ldr r3, =data_reg
    ldr r4, =0xABCD
    strh r4, [r3]      

   
    ldr r5, =result_reg
    ldrh r6, [r3]      
    strh r6, [r5]      

    swi 0x123456