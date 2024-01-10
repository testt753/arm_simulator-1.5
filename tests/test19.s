.global _start

.text
_start:
  @Initialize data
  ldr r0, =data
  ldr r1 , =#0x01234567
  mov r1, r1
  str r1, [r0]
  add r0, r0, #4
  ldr r1 , =#0x89ABCDEF
  mov r1, r1
  str r1, [r0]

  @Test ldrsb
  ldr r0, =data
  ldrsb r1, [r0]
  @r1 now contains the value 0x01

  @Test ldrsh
  ldr r0, =data + 1
  ldrsh r1, [r0]
  @r1 now contains the value 0x0123

   @Test strd and ldrd replaced with single word operations
  ldr r0, =data
  ldr r1 , =#0xDEADBEEF
  mov r1, r1
  str r1, [r0]
  add r0, r0, #4
  ldr r1 , =#0xCAFEBABE
  mov r1, r1
  str r1, [r0]

  ldr r0, =data
  ldr r1, [r0]
  @r1 now contains the value 0xDEADBEEF
  add r0, r0, #4
  ldr r2, [r0]
  @r2 now contains the value 0xCAFEBABE  
    swi 0x123456

.data
data: .space 8


@ce programme test les instructions ldrsb,ldrsh , et strd ,ldrd avec les instructions str et ldr en 2 paires(32bits)
@car le processeur ne prend pas en compte les instructions strd et ldrd