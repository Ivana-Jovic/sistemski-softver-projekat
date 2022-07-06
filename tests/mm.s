 .global a,c
.extern b
.section text
   jeq a
    jeq *e
    jeq b
    jeq *r0
d:  .word d
    add r0,r3
   add r6,r0
    add r0,r7
.section data
.skip 8
e:  .word a
    .word 7
    .word 0xff
a:  .word 4
call %e
 .equ m,0x17
call [r3+e]
.section bla
c:  .skip 8
    add r0,r7
.end