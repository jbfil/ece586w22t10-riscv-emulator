main:
    	addi a0, zero, 1
    	addi a1, zero, -1
    	blt  a0, a1, sl
    	bltu a0, a1, less
sl:
    	jr ra 
less:
	addi a0, zero , 2 
	jr ra 
