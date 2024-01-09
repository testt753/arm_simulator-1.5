.data
    q15_num1:   .hword 0x1234    @ Replace with your Q15 number
    q15_num2:   .hword 0x5678    @ Replace with another Q15 number
    q31_num1:   .word 0x12345678 @ Replace with your Q31 number
    q31_num2:   .word 0x87654321 @ Replace with another Q31 number

.text
.global _start

_start:
    @ Multiply the Q15 numbers in the bottom halves of R0 and R1 and place the Q31 result in R2
    ldr r0, q15_num1
    ldr r1, q15_num2
    smulbb r2, r0, r1
    qadd r2, r2, r2

    @ Multiply the Q31 number in R0 by the Q15 number in the top half of R1 and place the Q31 result in R2
    ldr r0, q31_num1
    ldr r1, q15_num2
    smulwt r2, r0, r1
    qadd r2, r2, r2

    @ Multiply the Q31 numbers in R0 and R1 and place the Q31 result in R2
    ldr r0, q31_num1
    ldr r1, q31_num2
    smull r3, r2, r0, r1
    qadd r2, r2, r2
    @ Your result is now in register R2

    swi 0x123456

