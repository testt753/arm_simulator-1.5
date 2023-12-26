.global main
.text
.data

main:
    ldr r0, =source_data
    ldrb r1, [r0]       ; Chargement d'un octet depuis la source
    strb r1, [r0, #4]   ; Stockage de l'octet dans la destination
    ldrh r2, [r0, #2]   ; Chargement d'un demi-mot depuis la source
    strh r2, [r0, #8]   ; Stockage du demi-mot dans la destination

    ldr r3, [r0, #4]    ; Chargement d'un mot depuis la source (byte offset)
    str r3, [r0, #12]   ; Stockage du mot dans la destination (halfword offset)

    ldrb r4, [r0, #3]   ; Chargement d'un octet depuis la source (halfword offset)
    strb r4, [r0, #15]  ; Stockage de l'octet dans la destination (word offset)

    ldmia r0!, {r5, r6} ; Chargement des mots suivants dans r5 et r6, et incrémentation de r0

    stmia r0!, {r7, r8} ; Stockage des mots dans r7 et r8, et incrémentation de r0

    swi 0x123456

source_data:
    .byte 0x42
    .byte 0x84
    .short 0x1234
    .short 0xABCD
    .word 0x11223344
    .word 0x55667788
