.global main
.text

main:
    mov r0, #2
    b branch_target
    mov r1, #10   

branch_target:
    mov r1, #8
    and r2, r0, r1    
    eor r3, r0, r1    
    sub r4, r0, r1    
    rsb r5, r0, r1    
    add r6, r0, r1    
    adc r7, r0, r1    
    sbc r8, r0, r1    
    rsc r9, r0, r1    
    orr r10, r0, r1   
    bic r11, r0, r1    

    swi 0x123456
