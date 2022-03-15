loop:
	ADDI x4, zero , 2
	ADDI x4,x4,12
	jr ra
main:
	ADDI s0, zero, 1
	ADD a1, zero, s0
	ADD a0, zero, s0
	ADDI x4, zero, 0
	JALR x5, 4(x4)
    ADDI x4, zero, 1
	jr ra
