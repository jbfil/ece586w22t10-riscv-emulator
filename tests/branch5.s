 main:
    addi a0, zero, 1
    addi a1, zero, 5
    blt  a0, a1, sl
    bltu a0, a1, less
sl:
    addi a2,zero, 1
less:
    addi a2, zero , 2
jr ra
