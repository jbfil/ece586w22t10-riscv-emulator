
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "memory.h"

int exec_inst32(u32 inst);
void match_inst32(u32 inst);

int load_mem(const char *filename);
void run_prog();

struct configuration config = {
	.verbose = 0,
	.inp_file = "program.mem",
	.start_addr = 0,
	.stack_addr = 0xFFFF,

	.echo_mem = 0,
	.step = 0
};


// Instantiate memory model.
union memory_model mem; // Memory.
s32 regs[32];           // 32 registers
u32 pc;                 // Program Counter.
u32 pc_next;            // Program Counter next value.


// Load the mem file into memory.
// example line "0: deadbeef"
int load_mem(const char *filename)
{
	FILE *fd;
	u32 offset, value;
	int res;

	// Zero memory Before Load.
	memset(mem.bytes, 0, sizeof(mem.bytes));
	fd = fopen(filename, "r");
	if (!fd)
		return -1;
	
	if (config.echo_mem) {
		errorf("Loading: %s\n", filename);
	}

	do {
		res = fscanf(fd, "%X: %X", &offset, &value);
		if (res != 2) {
			break;
		}

		if (config.echo_mem) {
			errorf("read: %4X: %08X\n", offset, value);
		}

		mem.words[offset >> 2] = value; // Assign the value.
		
	} while(1);

	fclose(fd);

	return 0;
}

const char * name_inst32(u32 inst)
{
	switch(inst & 0xFE00707F) { // Test Widest Opcodes.
		case 0x00005013: return "SRLI";
		case 0x40005013: return "SRAI";
		case 0x00000033: return "ADD ";
		case 0x40000033: return "SUB ";
		case 0x00005033: return "SRL ";
		case 0x40005033: return "SRA ";
	}
	switch(inst & 0x0000707F) {
		case 0x00000063: return "BEQ  ";
		case 0x00001063: return "BNE  ";
		case 0x00004063: return "BLT  ";
		case 0x00005063: return "BGE  ";
		case 0x00006063: return "BLTU ";
		case 0x00007063: return "BGEU ";
		case 0x00000003: return "LB   ";
		case 0x00001003: return "LH   ";
		case 0x00002003: return "LW   ";
		case 0x00004003: return "LBU  ";
		case 0x00005003: return "LHU  ";
		case 0x00000023: return "SB   ";
		case 0x00001023: return "SH   ";
		case 0x00002023: return "SW   ";
		case 0x00000013: return "ADDI ";
		case 0x00002013: return "SLTI ";
		case 0x00003013: return "SLTIU";
		case 0x00004013: return "XORI ";
		case 0x00006013: return "ORI  ";
		case 0x00007013: return "ANDI ";
		case 0x00001013: return "SLLI ";
		case 0x00001033: return "SLL  ";
		case 0x00002033: return "SLT  ";
		case 0x00003033: return "SLTU ";
		case 0x00004033: return "XOR  ";
		case 0x00006033: return "OR   ";
		case 0x00007033: return "AND  ";
	}
	switch (inst & 0x0000007F) {
		case 0x00000037: return "LUI  ";
		case 0x00000017: return "AUIPC";
		case 0x0000006F: return "JAL  ";
		case 0x00000067: return "JALR ";
	}

	return "Unknown";
}

void match_inst32(u32 inst)
{
	if (config.verbose >= 2 && config.show_masked) {
		errorf("Masked: 0x%08X 0x%08X 0x%08X\n", inst & 0xFE01F07F, inst & 0x0001F07F, inst & 0x0000007F);
	}
	switch(inst & 0xFE00707F) { // Test Widest Opcodes.
		case 0x00005013: process_SRLI(inst); return;
		case 0x40005013: process_SRAI(inst); return;
		case 0x00000033: process_ADD (inst); return;
		case 0x40000033: process_SUB (inst); return;
		case 0x00005033: process_SRL (inst); return;
		case 0x40005033: process_SRA (inst); return;
	}
	switch(inst & 0x0000707F) {
		case 0x00000063: process_BEQ  (inst); return;
		case 0x00001063: process_BNE  (inst); return;
		case 0x00004063: process_BLT  (inst); return;
		case 0x00005063: process_BGE  (inst); return;
		case 0x00006063: process_BLTU (inst); return;
		case 0x00007063: process_BGEU (inst); return;
		case 0x00000003: ltype_instr  (inst); return; // _LB 
		case 0x00001003: ltype_instr  (inst); return; // _LH 
		case 0x00002003: ltype_instr  (inst); return; // _LW 
		case 0x00004003: ltype_instr  (inst); return; // _LBU
		case 0x00005003: ltype_instr  (inst); return; // _LHU
		case 0x00000023: stype_instr  (inst); return; // _SB 
		case 0x00001023: stype_instr  (inst); return; // _SH 
		case 0x00002023: stype_instr  (inst); return; // _SW 
		case 0x00000013: process_ADDI (inst); return;
		case 0x00002013: process_SLTI (inst); return;
		case 0x00003013: process_SLTIU(inst); return;
		case 0x00004013: process_XORI (inst); return;
		case 0x00006013: process_ORI  (inst); return;
		case 0x00007013: process_ANDI (inst); return;
		case 0x00001013: process_SLLI (inst); return;
		case 0x00001033: process_SLL  (inst); return;
		case 0x00002033: process_SLT  (inst); return;
		case 0x00003033: process_SLTU (inst); return;
		case 0x00004033: process_XOR  (inst); return;
		case 0x00006033: process_OR   (inst); return;
		case 0x00007033: process_AND  (inst); return;
	}
	switch (inst & 0x0000007F) {
		case 0x00000037: process_LUI(inst); return;
		case 0x00000017: process_AUIPC(inst); return;
		case 0x0000006F: process_JAL(inst); return;
		case 0x00000067: process_JALR(inst); return;
	}

	errorf_exit("Unhandled instruction opcode. 0x%08X\n", inst);
}

int exec_inst32(u32 inst)
{
	if (config.verbose >= 2) {
		print_pc_inst_name(inst);
	} else if (config.verbose >= 1) {
		print_pc_inst(inst);
	}

	if (config.step) {
		getchar();
	}

	pc_next = pc + 4;

	if (config.use_match) match_inst32(inst);
	else                  inst_type(inst);

	if (pc_next % 4 != 0) { // Handle Missaligned
		errorf_exit("Exception instruction-address-missaligned PC= 0x%08X, target= 0x%08X\n", pc, pc_next);
	}
	pc = pc_next;
	
	if (config.verbose >= 1) {
		print_regs();
	}
	return 0;
}

void run_prog()
{
	while (pc < 0x10000) {
		// Fetch Instruction.
		u32 inst = mem.words[pc >> 2];

		// Decode Length.
		enum { len16, len32, len48, len64, undef } len = undef;
		if     ((inst & 0x03) != 0x03) len = len16;
		else if((inst & 0x1F) != 0x1F) len = len32;
		else if((inst & 0x3F) == 0x1F) len = len48;
		else if((inst & 0x7F) == 0x3F) len = len64;
		else {
			len = undef;
			errorf_exit("Unsupported length instruction. 0x%08X\n", inst);
		}

		// Process Instruction
		if (len == len32) { // Handle len32 instruction.
			exec_inst32(inst);
		} else {
			errorf_exit("Unhandled length instruction. 0x%08X\n", inst);
		}
	}

	errorf_exit("Memory Out of range. 0x%08X\n", pc);
}

void print_regs()
{
	printf("REGS:\t");
	for (int i = 0; i < 32; ++i) {
		if (i % 8 == 0 && i > 0) printf("\n\t");
		printf(" %08X", regs[i]);
	}
	printf("\n");
}

void print_pc()
{
	printf("PC: %08X\n", pc);
}

void print_pc_inst(u32 inst)
{
	printf("PC: %08X  INST: %08X\n", pc, inst);
}

void print_pc_inst_name(u32 inst)
{
	printf("PC: %08X  INST: %08X  %s\n", pc, inst, name_inst32(inst));
}


int main(int argc, char *argv[])
{
	// Parse arguments.
	for (int i = 1; i < argc; ++i) {
		char *arg = argv[i];
		if (arg[0] != '-') break;
		if (strequ(arg, "-v") || strequ(arg, "--verbose")) {
			if (config.verbose < 1) config.verbose = 1;
		}
		if (strequ(arg, "-d") || strequ(arg, "--debug")) {
			if (config.verbose < 2) config.verbose = 2;
		}
		if (strequ(arg, "-r") || strequ(arg, "--echo-mem")) {
			config.echo_mem = 1;
		}
		if (strequ(arg, "-i") || strequ(arg, "--input")) {
			++i; // Consume one argument.
			config.inp_file = argv[i];
		}
		if (strequ(arg, "-s") || strequ(arg, "--start_addr")) {
			++i; // Consume one argument.
			config.start_addr = strtoul(argv[i], NULL, 0); // Parse as flexible number format.
			assert(config.start_addr <= 0xFFFF); // Check address is valid.
		}
		if (strequ(arg, "-S") || strequ(arg, "--stack_addr")) {
			++i; // Consume one argument.
			config.stack_addr = strtoul(argv[i], NULL, 0); // Parse as flexible number format.
			assert(config.stack_addr <= 0xFFFF); // Check address is valid.
		}

		if (strequ(arg, "--step")) {
			config.step = 1;
		} else if (strequ(arg, "--show_masked")) {
			config.show_masked = 1;
		} else if (strequ(arg, "--show_details")) {
			config.show_details = 1;
		} else if (strequ(arg, "--use_match")) {
			config.use_match = 1;
		} else if (strequ(arg, "--use_matchn")) {
			config.use_match = 0;
		}
	}

	// Check Required Arguements.
	if (config.inp_file == NULL) {
		errorf("No input argument.\n");
		return -1;
	}

	// Process Program.
	load_mem(config.inp_file);

	// Intialize Program Values
	pc = config.start_addr;
	regs[SP] = config.stack_addr;

	// Run Program.
	run_prog();

	return 0;
}

