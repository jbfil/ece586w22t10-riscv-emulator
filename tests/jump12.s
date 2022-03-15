main:
	ADDI s0, zero, 1
	ADD a1, zero, s0
	ADD a0, zero, s0
	ADDI x4, zero, 0
	JALR x5, x4 , 0x18
	BEQ a1, a0 , loop
    ADDI x4, zero, 1
	jr ra 
loop:
	ADDI x4, zero , 2
	jr ra
