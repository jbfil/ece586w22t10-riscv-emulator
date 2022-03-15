
	ADDI t0, x0, -100
	ADDI t1, x0,   -1
	ADDI t2, x0,    0
	ADDI t3, x0,   10
	ADDI t4, x0,  100


	# Set expected in x32
	ADDI t6, x0, 0
	# Register Should be set to 0 after,
	SLTIU t5, t3, 0
	SLTI  t5, t3, 0
	SLTI  t5, t3, -1
	SLTI  t5, t3, -2047
	SLTI  t5, t3, -2048

	# Set expected in x32
	ADDI t6, x0, 1
	# Register Should be set to 1 after,
	SLTIU t5, t2, 1050
	SLTIU t5, t2, 2047

	SLTI  t5, t1, 2047
	SLTI  t5, t1, 10
	SLTI  t5, t1, 2047
	SLTI  t5, t1, 10   # Test nonzero.
