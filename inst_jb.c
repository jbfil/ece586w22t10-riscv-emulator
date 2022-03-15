

#include "common.h"
#include "memory.h"

void finish()
{
	pc = pc_next;
	print_results();
}

// Print Results.
void print_results()
{
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

	if (config.verbose >= 2 && config.show_details) {
		errorf("LUI rd= %d, val= 0x%08X\n", rd, val);
	}
	assert((val & 0xFFF) == 0x000); // Check lowest 12 bits are zero.
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
	s32 offset = (imm & (1 << 20) ? 0xFFF00000 : 0) | imm;

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

	u32 imm = inst32.Itype.imm; // Sign extended 12 bit immediate, set lsb to 0
	s32 offset = ((imm & (1 << 11) ? 0xFFFFF000 : 0) | imm) & 0xFFFFFFFE;

	// Save PC + 4 into register.
	if (rd != 0) regs[rd] = pc + 4;

	// Compute and set jump,
	pc_next = regs[rs1] + offset;

	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, imm= 0x%08X, offset= %d\n", __FUNCTION__, rs1, imm, offset);
	}

	if (pc_next == 0) finish();
}

s32 signExtend(u32 imm, u32 msb, u32 lsb)
{
	assert(msb > lsb);
	assert(msb < 32);
	const u32 mask = ((1 << msb) - 1) & ~(1 << lsb - 1);
	assert((imm & mask) == imm); // Check all values are in declared range.
	const u32 extend = 0xFFFFFFFF & ~((1 << msb) - 1);

	return (imm & (1 << msb)) ? extend | imm : imm;
}

s32 signExtendImm12(u32 imm)
{
	assert((imm & 0x00001FFE) == imm); // Check value is in bits 12:1 
	return (imm & (1 << 12) ? 0xFFFFE000 : 0) | imm;
}

s32 Btype_offset(u32 inst)
{
	union riscv_inst32 inst32 = { inst };
	u32 imm =
		(inst32.Btype.imm12 << 12) |
		(inst32.Btype.imm05 <<  5) |
		(inst32.Btype.imm11 << 11) |
		(inst32.Btype.imm01 <<  1);
	s32 offset = signExtendImm12(imm);
	return offset;
}

void process_BEQ  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rs1 = inst32.Btype.rs1;
	int rs2 = inst32.Btype.rs2;

	s32 a = ((s32*)regs)[rs1];
	s32 b = ((s32*)regs)[rs2];
	s32 offset = Btype_offset(inst);
	int res = (a == b);

	if (res) pc_next = pc + offset;
	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, rs2= %d, res= %d, offset= 0x%08X\n", __FUNCTION__, rs1, rs2, res, offset);
	}
}

void process_BNE  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rs1 = inst32.Btype.rs1;
	int rs2 = inst32.Btype.rs2;

	s32 a = ((s32*)regs)[rs1];
	s32 b = ((s32*)regs)[rs2];
	s32 offset = Btype_offset(inst);
	int res = (a != b);

	if (res) pc_next = pc + offset;
	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, rs2= %d, res= %d, offset= 0x%08X\n", __FUNCTION__, rs1, rs2, res, offset);
	}
}

void process_BLT  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rs1 = inst32.Btype.rs1;
	int rs2 = inst32.Btype.rs2;

	s32 a = ((s32*)regs)[rs1];
	s32 b = ((s32*)regs)[rs2];
	s32 offset = Btype_offset(inst);
	int res = (a < b);

	if (res) pc_next = pc + offset;
	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, rs2= %d, res= %d, offset= 0x%08X\n", __FUNCTION__, rs1, rs2, res, offset);
	}
}

void process_BGE  (u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rs1 = inst32.Btype.rs1;
	int rs2 = inst32.Btype.rs2;

	s32 a = ((s32*)regs)[rs1];
	s32 b = ((s32*)regs)[rs2];
	s32 offset = Btype_offset(inst);
	int res = (a >= b);

	if (res) pc_next = pc + offset;
	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, rs2= %d, res= %d, offset= 0x%08X\n", __FUNCTION__, rs1, rs2, res, offset);
	}
}

void process_BLTU (u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rs1 = inst32.Btype.rs1;
	int rs2 = inst32.Btype.rs2;

	u32 a = regs[rs1];
	u32 b = regs[rs2];
	s32 offset = Btype_offset(inst);
	int res = (a < b);

	if (res) pc_next = pc + offset;
	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, rs2= %d, res= %d, offset= 0x%08X\n", __FUNCTION__, rs1, rs2, res, offset);
	}
}

void process_BGEU (u32 inst)
{
	union riscv_inst32 inst32 = { inst };

	int rs1 = inst32.Btype.rs1;
	int rs2 = inst32.Btype.rs2;

	u32 a = regs[rs1];
	u32 b = regs[rs2];
	s32 offset = Btype_offset(inst);
	int res = (a >= b);

	if (res) pc_next = pc + offset;
	if (config.verbose >= 2 && config.show_details) {
		errorf("show_details: %s rs1= %d, rs2= %d, res= %d, offset= 0x%08X\n", __FUNCTION__, rs1, rs2, res, offset);
	}
}


void unit_test_jb() {


}

