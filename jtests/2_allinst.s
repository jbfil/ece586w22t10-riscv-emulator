
# This test enumerates all of the instructions.

	LUI  x0, 10
	AUIPC x0, 12

	LB   x0, 0(x0)
	LH   x0, 0(x0)
	LW   x0, 0(x0)
	LBU  x0, 0(x0)
	LHU  x0, 0(x0)
	SB   x0, 0(x0)
	SH   x0, 0(x0)
	SW   x0, 0(x0)

	ADDI x0, x0, 1
	SLTI x0, x0, 1
	SLTIU x0, x0, 1
	XORI x0, x0, 1
	ORI x0, x0, 1
	ANDI x0, x0, 1
	SLLI x0, x0, 1
	SRLI x0, x0, 1
	SRAI x0, x0, 1

	ADD x0, x0, x0
	SUB x0, x0, x0
	SLL x0, x0, x0
	SLT x0, x0, x0
	SLTU x0, x0, x0
	XOR x0, x0, x0
	SRL x0, x0, x0
	SRA x0, x0, x0
	OR x0, x0, x0
	AND x0, x0, x0

	ADDI t0, x0, 0
	ADDI t1, x0, 1

	BEQ  x0, t1, end
	BNE  x0, t0, end
	BLT  t1, x1, end
	BGE  x0, t1, end
	BLTU t1, x0, end
	BGEU x0, t1, end

	JAL x0, end
end:
	jr ra
