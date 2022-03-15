
main:
	# Check Immediate loading.
	ADDI t0, x0, ( 0)
	ADDI t1, x0, ( 1)
	ADDI t2, x0, (-1)

	# Check branch goes to start. Check offset is negative of PC
	BEQ  x0, t1, main

	BEQ  x0, t1, not_taken
not_taken:
	# None of the branches taken
	BEQ  x0, t1, not_taken
	BNE  x0, t0, not_taken
	BLT  t1, x1, not_taken
	BGE  x0, t1, not_taken
	BLTU t1, x0, not_taken
	BGEU x0, t1, not_taken

taken:
	# All branches taken

	jr ra


