
jpos:   
	
	JAL ra, j0
	JAL ra, j1
	JAL ra, j2
	JAL ra, j3

	JALR ra, x0, 48
	JALR ra, x0, 64
	JALR ra, x0, 80
	JALR ra, x0, 96

	J jnegp
	NOP
	NOP
	NOP

	# 16 targets that just return.
j0:	RET
	RET
	RET
	RET
j1:	RET
	RET
	RET
	RET
j2:     RET
	RET
	RET
	RET
j3:	RET
	RET
	RET
	RET

jneg:	JAL ra, j0
	JAL ra, j1
	JAL ra, j2
	JAL ra, j3

	JALR ra, t0, -64
	JALR ra, t0, -48
	JALR ra, t0, -32
	JALR ra, t0, -16

	ADD ra, x0, x0
	JR ra
	NOP
	NOP

jnegp:  la   t0, jneg
	la   t1, j0
	la   t2, j1
	J jneg

