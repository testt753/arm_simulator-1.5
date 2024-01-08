.global main
.text

main:
    mov r0, #0xFF00FF00
    clz r1, r0  // Compte les zéros en tête de r0 et stocke le résultat dans r1

   mov r0, #0x01234567 
   blx function1       
   bl function2       
   bx lr              

function1:
   add r0, r0, r0     
   bx lr              

function2:
   sub r0, r0, #1     
   bx lr              

    swi 0x123456