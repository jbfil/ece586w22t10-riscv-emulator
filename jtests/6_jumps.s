
# Test Jumps

j0:     JAL x0, j1 # Jump to skip
	LUI x7, 0xFFFF
j1:     SW  ra, -4(sp)
	JAL ra, func1
	LW  ra, -4(sp)

j2:     SW  ra, -4(sp)
	JAL ra, func2
	LUI x7, 0xFFFF # Skip this returning from func2
	LW  ra, -4(sp)

	JALR x0, 100(x0) # Jump into the blue.
	JR ra          # End simulation

func1:
	LUI x7, 0x1
	JALR x0, 0(ra)

func2:
	LUI x7, 0x1
	JALR x0, 4(ra)



	

