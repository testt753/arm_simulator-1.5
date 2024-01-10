.global main
main:                   @tester stmdb
    ldr r0, =adress_stoc
    ldmia r0!, {r5, r6} @r5 et r6 prend les valeurs depuis la memoire
    mov r1, #1 @ r1=1
    add r2, r1, #2 @r2=3
    sub r3, r2, r1 @r3=2
    mul r4, r2, r3 @r4=6
    stmdb r0!, {r1, r2, r3}
bl loop
add r4, r4, #30     @r4= 33
str r4, [r0, #12]
b end
loop:
cmp r4, r2
beq endloop
sub r4, r4, #1
b loop
endloop:
    bx lr

.data
    adress_stoc: .word 10, 20
end: