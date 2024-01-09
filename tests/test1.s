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
    adc r6, r0, #3     
    sbc r7, r0, #8     
    rsc r8, r0, #12    
    tst r0, #42        
    teq r0, #0         
    cmp r0, #20        
    cmn r0, #5         
    orr r9, r0, #128    
    bic r11, r0, #7     
    mvn r12, r0         

    swi 0x123456