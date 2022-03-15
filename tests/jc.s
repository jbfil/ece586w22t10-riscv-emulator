fast_multiply:
	ADD a1, a0, a1
	JR ra
main:
	ADDI s0, zero, 1
	ADD a0, zero, s0
	ADD a1, zero, a0
	JAL ra, fast_multiply
	BEQ a1, a0 , loop
    ADDI ra, ra , 0
	JR ra
loop:
   ADDI a1, zero ,10
   JR ra
