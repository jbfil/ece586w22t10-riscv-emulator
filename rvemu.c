
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t	u08;
typedef uint32_t u32;
typedef int32_t	s32;

#include "common.h"

// RISC-V Instruction len32
union riscv_inst32 {
	u32 inst;

	struct __attribute__((packed)) {
		u32 opcode : 7;
	} any;

	struct __attribute__((packed)) {
		u32 opcode : 7;
		u32 rd     : 5;
		u32 funct3 : 3;
		u32 rs1    : 5;
		u32 rs2    : 5;
		u32 funct7 : 7;
	} Rtype;

	struct __attribute__((packed)) {
		u32 opcode : 7;
		u32 rd     : 5;
		u32 funct3 : 3;
		u32 rs1    : 5;
		u32 imm    :12;
	} Itype;

	struct __attribute__((packed)) {
		u32 opcode : 7;
		u32 imm_l  : 5;
		u32 funct3 : 3;
		u32 rs1    : 5;
		u32 rs2    : 5;
		u32 imm    : 7;
	} Stype;

	struct __attribute__((packed)) {
		u32 opcode : 7;
		u32 rd     : 5;
		u32 imm    :20;
	} Utype;

	// TODO: B and J formats.
};

// Memory Model
union {
	u08 bytes[1 << 16]; // View as bytes.
	u32 words[1 << 13]; // View as words.
	u32 swords[1 << 13]; // View as signed words.
} mem;

// Registers Model
s32 regs[32]; // 32 registers

struct {
	int verbose;
	int echo_mem;
	const char *inp_file;
} config = {0};

// Load the mem file into memory.
// example line "0: deadbeef"
int load_mem(const char *filename)
{
	FILE *fd;
	u32 offset, value;
	int res;

	memset(mem.bytes, 0, sizeof(mem.bytes));
	fd = fopen(filename, "r");
	if (!fd)
		return -1;

	do {
		res = fscanf(fd, "%X: %X", &offset, &value);
		if (res != 2) {
			break;
		}

		if (config.echo_mem) {
			errorf("%4X: %08X\n", offset, value);
		}

		mem.words[offset >> 3] = value; // Assign the value.
		
	} while(1);

	fclose(fd);

	return 0;
}

int run_prog(u32 pc_unsafe) {
	u32 pc = pc_unsafe & (sizeof(mem.bytes) - 1); // Mask PC to power of 2 size.
	if( pc != pc_unsafe) {
		errorf_exit("Memory Out of range. 0x%08X\n", pc_unsafe);
	}

	// Fetch Instruction.
	u32 inst = mem.words[pc];
	
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
		union riscv_inst32 inst32 = { inst };

		switch (inst32.any.opcode) {
		default:
			errorf_exit("Unhandled instruction opcode. 0x%08X\n", inst);
		}
	} else {
		errorf_exit("Unhandled length instruction. 0x%08X\n", inst);
	}
	
	
	return 0;
}


int main(int argc, char *argv[])
{
	// Parse arguments.
	for (int i = 1; i < argc; ++i) {
		char *arg = argv[i];
		if (arg[0] != '-') break;
		if (strequ(arg, "-v")) {
			config.verbose += 1;
		}
		if (strequ(arg, "-r")) {
			config.echo_mem = 1;
		}
		if (strequ(arg, "-i") || strequ(arg, "--input")) {
			++i; // Consume one argument.
			config.inp_file = argv[i];
		}
	}

	// Check Required Arguements.
	if (config.inp_file == NULL) {
		errorf("No input argument.\n");
		return -1;
	}

	// Process Program.
	load_mem(config.inp_file);
	
	return 0;
}

