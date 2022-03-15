
# Test Load Upper Immediate
	LUI x7, 0
	LUI x7, 1
	LUI x7, 0xFFF
	LUI x7, 0xFFFFF

# Test AUIPC
	AUIPC x7, 0        # Expect PC + immeidate in upper
	AUIPC x7, 0xFF000
	AUIPC x7, 0xFFFFF

# Test AUIPC higher ofsets when PC is much larger.

	


	JR ra          # End simulation

