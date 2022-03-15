main:
   addi a0, zero, 5
    addi a1, zero, 3
    bge a0, a1, gt
    addi a0 , zero , 1
gt:
    addi a0, zero, 10 
jr ra
