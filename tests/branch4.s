main:
    addi a0, zero, 3
    addi a1, zero, 3
    blt a0, a1, ls
    beq a0, a1, equal
ls: 
    sub a2, a1, a0 
equal:
     add a2, a0, a1
jr ra
