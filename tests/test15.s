.global main
.text

function1:
   add r0, r0, r0     
   bx lr              

function2:
   sub r0, r0, #1     
   bx lr 

main:
    mov r0, #0xFF00FF00
    clz r1, r0  

   mov r0, #0x01234567 
   blx function1       
   bl function2       
   bx lr              
   swi 0x123456