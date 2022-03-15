 main:
	addi a0, zero, -1
	addi a1, zero, 1
	bgeu a0, a1 , sg 
	addi a2, zero , 1 
sg:
	addi a2, zero, 2 
jr ra
