

#include "common.h"
#include "memory.h"

void finish()
{
	pc = pc_next;
	// Print Results.
	print_pc();
	print_regs();
	exit(0);
}

void process_LUI(u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	// Write to upper bits of register. Clobber rest with zeros.
	int rd = inst32.Utype.rd;
	u32 val = inst32.Utype.imm << 12;

	if (rd != 0) regs[rd] = val;

	assert(val & 0xFFF == 0x000); // Check lowest 12 bits are zero.
}

void process_AUIPC(u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	// Create a PC realative address.
	int rd = inst32.Utype.rd;
	u32 val = inst32.Utype.imm;
	u32 addr = pc + (val << 12);

	if (rd != 0) regs[rd] = addr;
}

void process_JAL(u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rd = inst32.Jtype.rd;

	// Signed offset in multiples of 2.
	u32 imm =
		(inst32.Jtype.imm20 << 20) |
		(inst32.Jtype.imm12 << 12) |
		(inst32.Jtype.imm11 << 11) |
		(inst32.Jtype.imm01 <<  1);
	s32 offset = (imm & (1 << 19) ? 0xFFF00000 : 0) | imm;

	// Save PC + 4 into register.
	if (rd != 0) regs[rd] = pc + 4;

	// Compute and set jump,
	pc_next = pc + offset;

	if (pc_next == 0) finish();
}

void process_JALR(u32 inst)
{
	union riscv_inst32 inst32 = { inst };
	int rd = inst32.Itype.rd;
	int rs1 = inst32.Itype.rs1;

	u32 imm = inst32.Itype.rs1; // Sign extended 12 bit immediate, set lsb to 0
	s32 offset = ((imm & (1 << 11) ? 0xFFFFF000 : 0) | imm) & 0xFFFFFFFE;

	// Save PC + 4 into register.
	if (rd != 0) regs[rd] = pc + 4;

	// Compute and set jump,
	pc_next = regs[rs1];//+ offset;

	if (pc_next == 0) finish();
}

void process_BEQ  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };
}

void process_BNE  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };
}

void process_BLT  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };
}

void process_BGE  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };
}

void process_BLTU (u32 inst)
{
	union riscv_inst32 inst32 = { inst };
}

void process_BGEU (u32 inst)
{
	union riscv_inst32 inst32 = { inst };
}


