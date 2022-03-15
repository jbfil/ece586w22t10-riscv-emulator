
// Configuration structure.
struct configuration {
	int verbose; // Sets output mode.
	// 0 - silent mode. output only PC of final instruction.
	// 1 - verbose mode. output hex value of each instruction.
	// 2 - debug mode. output extra information.

	const char *inp_file;
	u32 stack_addr;
	s32 start_addr;

	int echo_mem;
	int step;
	int show_masked;
	int show_details;
	int use_match;
};

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

	struct __attribute__((packed)) {
		u32 opcode : 7;
		u32 rd     : 5;
		u32 imm12  : 8;
		u32 imm11  : 1;
		u32 imm01  :10;
		u32 imm20  : 1;
	} Jtype;

	struct __attribute__((packed)) {
		u32 opcode : 7;
		u32 imm11  : 1;
		u32 imm01  : 4;
		u32 funct3 : 3;
		u32 rs1    : 5;
		u32 rs2    : 5;
		u32 imm05  : 6;
		u32 imm12  : 1;
	} Btype;
};

// Memory Model
union memory_model {
	u08 bytes[1 << 16]; // View as bytes.
	u32 words[1 << 14]; // View as words.
	u16 hwords[1 << 15]; // View as half words.
};

// Registers Model
enum regs_indexs {
	ZERO, RA, SP,  GP,  TP, T0, T1, T2,
	S0,   S1, A0,  A1,  A2, A3, A4, A5,
	A6,   A7, S2,  S3,  S4, S5, S6, S7,
	S8,   S9, S10, S11, T3, T4, T5, T6,
};

extern union memory_model mem; // Memory.
extern s32 regs[32];           // 32 registers
extern u32 pc;                 // Program Counter.
extern u32 pc_next;            // Program Counter Next value.
extern struct configuration config; // Program options.


void print_regs();
void print_pc();
void print_pc_inst(u32 inst);
void print_pc_inst_name(u32 inst);

void match_inst32(u32 inst);
void  inst_type (u32 inst);
void  ltype_instr (u32 inst);
void  stype_instr (u32 inst);

// Instructions
void process_SLL(u32 inst);
void process_SLT(u32 inst);
void process_SRL(u32 inst);
void process_SRA(u32 inst);
void process_SLTU(u32 inst);
void process_SLTIU(u32 inst);
void process_SLTI(u32 inst);
void process_SRAI(u32 inst);
void process_SRLI(u32 inst);
void process_SLLI(u32 inst);

void process_ANDI(u32 inst);
void process_ORI(u32 inst);
void process_XORI(u32 inst);
void process_ADDI(u32 inst);
void process_AND(u32 inst);
void process_OR(u32 inst);
void process_XOR(u32 inst);
void process_SUB(u32 inst);
void process_ADD(u32 inst)  ;

void process_LUI  (u32 inst);
void process_AUIPC(u32 inst);
void process_JAL  (u32 inst);
void process_JALR (u32 inst);

void process_BEQ  (u32 isnt);
void process_BNE  (u32 isnt);
void process_BLT  (u32 isnt);
void process_BGE  (u32 isnt);
void process_BLTU (u32 isnt);
void process_BGEU (u32 isnt);


