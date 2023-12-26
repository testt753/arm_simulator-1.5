.global main
.text

main:
    mov r0, #2
    b branch_target
    mov r1, #10   

branch_target:
    mov r1, #8
    and r2, r0, r1    ; r2 = r0 AND r1
    eor r3, r0, r1    ; r3 = r0 EOR r1
    sub r4, r0, r1    ; r4 = r0 - r1
    rsb r5, r0, r1    ; r5 = r1 - r0
    add r6, r0, r1    ; r6 = r0 + r1
    adc r7, r0, r1    ; r7 = r0 + r1 + Carry
    sbc r8, r0, r1    ; r8 = r0 - r1 - (1 - Carry)
    rsc r9, r0, r1    ; r9 = r1 - r0 - (1 - Carry)
    orr r10, r0, r1   ; r10 = r0 OR r1
    bic r11, r0, r1   ; r11 = r0 AND NOT r1
    mvn r12, r0, r1   ; r12 = NOT r0

    swi 0x123456
