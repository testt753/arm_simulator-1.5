.global main 
.text

main:
    b branch_target
    mov r0, #2

branch_target:
    mov r0, #42
    and r1, r0, #15    
    eor r2, r0, #25    
    sub r3, r0, #10    
    rsb r4, r0, #30    
    add r5, r0, #5     
    adc r6, r0, #3     ; r6 = r0 + 3 + Carry
    sbc r7, r0, #8     ; r7 = r0 - 8 - (1 - Carry)
    rsc r8, r0, #12    ; r8 = 12 - r0 - (1 - Carry)
    tst r0, #42         ; Teste r0 AND 42, mets à jour les flags sans modifier les registres de destination
    teq r0, #0          ; Teste r0 EOR 0, mets à jour les flags sans modifier les registres de destination
    cmp r0, #20         ; Compare r0 - 20, mets à jour les flags sans modifier les registres de destination
    cmn r0, #5          ; Compare r0 + 5, mets à jour les flags sans modifier les registres de destination
    orr r9, r0, #128    ; r9 = r0 OR 128
    bic r11, r0, #7     ; r11 = r0 AND NOT 7
    mvn r12, r0         ; r12 = NOT r0

    swi 0x123456