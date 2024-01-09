.global main
.text
function:
  add r0, r0, #5    
  bx lr
main:
    ldr r1 , =#0xFF00FF00
   mov r0, r1
   clz r2, r0  @Compte les zéros en tête de r0 et stocke le résultat dans r1
 
  ldr r3 , =#0x01234567
  mov r0, r3
  bl function              

    swi 0x123456