#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "memory.h"


void increment_pc();
void inst_type (u32 inst);
void Rtype_inst(u32 inst);
void Itype_inst(u32 inst);
void ltype_instr(u32 instr);
void stype_instr(u32 instr);

u08  get_memb(u16 addr);
u32 get_mem(u16 addr);
u16 get_memh(u16 addr);
void save_mword(u16 addr, u32 reg_val);
void save_mhw(u16 addr, u16 reg_hval);
void save_mbyte(u16 addr,u08 reg_bval);

u32 get_reg(u32 addr);
void save_reg(u32 addr,u32 value);
s32 signExtension(u32 inst);


int main_xx()
{
	int i;
//uint32_t inst =0b00000000101010001001000000110011;
//uint32_t inst =0b00000001010111100010101100110011;//For SLT

//	u32 inst=0b00000001011001010101111000110011;//For SRL
//	u32 inst=0x40005033;//For SRA
//u32 inst=0b01000000000101011101110110110011;//SRA
//u32 inst=0b00000000000101011011110110110011;//SLTU
  u32 inst=0b01001101010110100010110010010011;//SLTI
//uint32_t inst =0b00000001010111100010101100110011;//For SLT
   // uint32_t inst = 0x0a11b0ff; //0000101 00001 00011 011 00001 1111111
   for(i=0;i<32;i++)
{
    regs[i]=i;

}

 memset(mem.bytes, 0, sizeof(mem.bytes));

union riscv_inst32 inst32 = { inst };
inst_type(inst);

printf("inst32 rs1: %u\n",inst32.Rtype.rs1);
printf("inst32 rs2: %u\n",inst32.Rtype.rs2);

printf("inst32 value in rtype format: %u\n",inst32.Rtype.funct7);
printf("inst32 funct3 value in rtype format: %u\n",inst32.Rtype.funct3);
printf("inst32 value in utype is : %u\n",inst32.Utype.imm);
printf("value of mem addr c is %d %d, %d",mem.bytes[11], mem.hwords[6],mem.words[3]);

printf("%u,%u,%u",inst32.Rtype.opcode,inst32.Rtype.funct3,inst32.Rtype.funct7);
printf("val of reg 10 and 17 is : %d and %d",regs[inst32.Rtype.rs2],regs[17]);
return 0;



	// TODO: B and L formats.
};



void  inst_type (u32 inst)
 {
     union riscv_inst32 inst32 = { inst };
	//u08  opcode = extract_opcode(inst);

	switch (inst32.any.opcode){
	 /*   case 0b0110011:  Rtype_inst(inst);   //  r type
			       //  increment_pc();
			        break;
		case 0b00010011:  Itype_inst(inst);  // immediate type
			        // increment_pc();
			        break;	
					*/  
				      
	case 0b00000011:  ltype_instr(inst);  // load type
			        increment_pc();
			        break;

	case 0b00010011:  Itype_inst(inst);  // immediate type
			         increment_pc();
			        break;
	case 0b00100011 :  stype_instr(inst);  // s type
			          increment_pc();
			        	break;
	case 0b00110011:  Rtype_inst(inst);   //  r type
			         increment_pc();
			        break;
			        /*
	case 0b00110111: u_lui_inst(inst);   //  lui
				 increment_pc();
			        break;
	case 0b00010111:  u_auipc_inst(inst);  //  auipc
				break;
	case 0b01101111:    jtype_inst(inst);  //  j type
				break;
case 0b01100011:   btype_inst(inst); // branch type
			break;
case 0b01110011:  e_inst(inst);  //ebreak and ecall
			break;
*/		
default:  printf("error :  opcode not supported\n");
}

}


void increment_pc()
{
	pc  = pc + 4;
}


// R Type:

	void Rtype_inst(u32 inst)
	{
		union riscv_inst32 inst32 = { inst };
		switch(inst32.Rtype.funct3)
		{
		    case 0b001:
				if(inst32.Rtype.funct7==0b0000000)
					process_SLL(inst);
				else
					printf ("No function has been defined for this value of funct7(001) ");
				break;
				

			case 0b011:
				if(inst32.Rtype.funct7==0b0000000)
					process_SLTU(inst);
				else
					printf ("No function has been defined for this value of funct7(011) ");
				break;				
			
			case 0b000:
				if(inst32.Rtype.funct7==0b0100000)
					process_SUB(inst);
				else if(inst32.Rtype.funct7==0b0000000)
					process_ADD(inst);
				else
					printf ("No function has been defined for this value of funct7(000) ");
				break;


			case 0b010:
				if(inst32.Rtype.funct7==0b0000000)
					process_SLT(inst);
				else
					printf ("No function has been defined for this value of funct7(010) ");
				break;


			case 0b100:
				if(inst32.Rtype.funct7==0b0000000)
					process_XOR(inst);
				else
					printf ("No function has been defined for this value of funct7(100) ");
				break;

			case 0b101:
				if(inst32.Rtype.funct7==0b0100000)
					process_SRA(inst);
				else if(inst32.Rtype.funct7==0b0000000)
					process_SRL(inst);
				else
					printf ("No function has been defined for this value of funct7(101) ");
				break;

			case 0b110:
				if(inst32.Rtype.funct7==0b0000000)
					process_OR(inst);
				else
					printf ("No function has been defined for this value of funct7(110) ");
				break;

			case 0b111:
				if(inst32.Rtype.funct7==0b0000000)
					process_AND(inst);
				else
					printf ("No function has been defined for this value of funct7(111) ");
				break;

			default: printf("No function has been defined for this value of funct3");

		}
	}


	void Itype_inst(u32 inst)
	{
	    union riscv_inst32 inst32 = { inst };
		switch(inst32.Itype.funct3)
		{			
			case 0b000: process_ADDI(inst);
						break;
			case 0b010: process_SLTI(inst);
						break;
			case 0b011: process_SLTUI(inst);
						break;
			case 0b100: process_XORI(inst);
						break;
			case 0b110: process_ORI(inst);
						break;
			case 0b111: process_ANDI(inst);
						break;
			case 0b001:
				if(inst32.Rtype.funct7==0b0000000)
					process_SLLI(inst);
				else
					printf ("No function has been defined for this value of funct7(001) ");
				break;
			case 0b101:
				if(inst32.Rtype.funct7==0b0100000)
					process_SRAI(inst);
				else if(inst32.Rtype.funct7==0b0000000)
					process_SRLI(inst);
				else
					printf ("No function has been defined for this value of funct7(101) ");
				break;

			default: printf("No function has been defined for this value of funct3");

		}

	}
void ltype_instr(u32 instr) {
	union riscv_inst32 loadInst = { instr };
	u32 base = regs[loadInst.Itype.rs1];  
	u32 effective_addr = base + signExtension((u32) (loadInst.Itype.imm)); 
	u32 val_word;
	u16  val_hw; 
	u08  val_byte;
	if(effective_addr <= 0xFFFF)
    {
	
	switch(loadInst.Itype.funct3) {
	case 0b000:   val_byte = get_memb(effective_addr);
		if((((val_byte & (1<<7)) >> 7 ) == 1) )
		{
		        val_word = 0xffffff00 | val_byte;
	}
	else {
		val_word = (u32) (val_byte);
	}
// save_reg
		save_reg(loadInst.Itype.rd,val_word);
		break;
	case 0b001:	val_hw = get_memh(effective_addr);
		if((((val_hw & (1<<15)) >> 15 ) == 1)) 
		{
		        val_word = 0xffff0000 | val_hw;
	}
	else {
		val_word = (u32) (val_hw);
	}
// save_reg
		save_reg(loadInst.Itype.rd,val_word);
	break;

	case 0b010:    val_word = get_mem(effective_addr);
				
		// save_reg
		save_reg(loadInst.Itype.rd,val_word);
	break;

	case 0b100:    val_byte = get_memb(effective_addr);
		val_word = (u32) (val_byte);
		save_reg(loadInst.Itype.rd,val_word);
		break;


	case 0b101:	val_hw = get_memh(effective_addr);
		val_word = (u32) (val_hw);
		save_reg(loadInst.Itype.rd,val_word);
		break;

	default: 	printf("not a valid load type");
				break;
	}
	}
	printf("effective addr out of bounds.");
}


void stype_instr(u32 instr)
{
        union riscv_inst32 storeInst = {instr};
        u16 imm =  (u16)(storeInst.Stype.imm<<5) | (u16)(storeInst.Stype.imm_l);
        u32 base =   get_reg(storeInst.Stype.rs1); 
        u32 reg_val = get_reg(storeInst.Stype.rs2);
        u16 reg_hval = (u16) reg_val;
        u08 reg_bval = (u08) reg_val;
        u32 effective_addr = base + signExtension((u32) (imm)); 
    if(effective_addr <= 0xFFFF) // not needed 
    {
	
    switch(storeInst.Stype.funct3) 
	{

   	case 0b000:	save_mbyte(effective_addr,reg_bval); //byte to mem
					break; 
	case 0b001:	save_mhw(effective_addr,reg_hval); //hw to mem
					break; 
	case 0b010: save_mword(effective_addr,reg_val);  //word to mem
					break;
	default:    printf("store type not supported");
					break;
}}
	else 
		printf("effective addr out of bounds");
}


u32 get_reg(u32 addr)
{
	
	if(addr<32)
{
		return regs[addr];
	}
		
	else
		printf("Register's address invalid");
		return -1;
}

 void save_reg(u32 addr,u32 value)
{

	if(addr<32 && addr !=0)
		 regs[addr]=value;

	else
	{
		if (addr == 0)
			printf ("Warning! We cannot write to Register R0");
		else
			printf("Address is out of range");
	}
	
}

void save_mbyte(u16 addr,u08 reg_bval){
mem.bytes[addr] = reg_bval;	
}

void save_mhw(u16 addr, u16 reg_hval){
	if((addr & 0x1) == 0)
		mem.hwords[addr>>1] = reg_hval; 
	else{
	
		printf("Error: address is not half word alligned.");
		
		
}
}

void save_mword(u16 addr, u32 reg_val) {
	if((addr & 0x3) == 0)
		mem.words[addr>>2] = reg_val; 
	else {
		printf("Error: address is not word alligned.");
		
}

}

u16 get_memh(u16 addr)
{
	if((addr & 0x1) == 0) //add check for addr not out of bounds
return mem.hwords[addr>>1];
else 
	{
		printf("Error: address is not half word alligned.");
		return -1;

}

}

u32 get_mem(u16 addr) {
	printf("anding is %d\n",(addr & 0x3));
	if((addr & 0x3) == 0)
		return mem.words[addr>>2];
else {
		
		printf("Error: address is not word alligned.");
		return -1;
}

}

u08  get_memb(u16 addr) {
	
	return mem.bytes[addr];

}

u32 get_shamt(inst)
{
	u32 temp=inst>>20;
	u32 shamt= (((1<<5)-1) & temp);

	if(temp>>4==1)
	{
		printf("Shift amount is negative");
		return -1;
	}
	else 
		return shamt;
}


 s32 signExtension(u32 inst)
{
    s32 value = (0x00000FFF & inst);
    u32 mask = 0x00000800;
    u32 sign = (mask & inst) >> 11;
    if (sign == 1)
        value += 0xFFFFF000;
    return value;
}

void process_SLL(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_reg(instr.Rtype.rs2);
	u32 val3=val1 << val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value %u\n",val3);
}

void process_SLT(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_reg(instr.Rtype.rs2);
	u32 val3=((s32) val1 <(s32) val2)?(u32) 1:(u32)0;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value %u\n",val3);
}
void process_SRL(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_reg(instr.Rtype.rs2);
	u32 val3=val1 >> val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value %u\n",val3);
}


void process_SRA(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_reg(instr.Rtype.rs2);
	u32 val3= ((s32)val1) >> val2;
	save_reg(instr.Rtype.rd,val3);
	printf("SRA Final value %u\n",val3);
}


void process_SLTU(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_reg(instr.Rtype.rs2);
	u32 val3=(val1 <val2)?(u32) 1:(u32)0;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value %u\n",val3);
}

void process_SLTI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=signExtension((u32)instr.Itype.imm);
	u32 val3=((s32) val1 <(s32) val2)?(u32) 1:(u32)0;
	save_reg(instr.Itype.rd,val3);
	printf("Final value %u\n",val3);
}

void process_SLTUI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=signExtension((u32)instr.Itype.imm);
	u32 val3=(val1 <val2)?(u32) 1:(u32)0;
	save_reg(instr.Itype.rd,val3);
	printf("Final value %u\n",val3);
}

void process_SLLI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_shamt(inst);
	u32 val3=val1 << val2;
	save_reg(instr.Itype.rd,val3);
	printf("Final value %u\n",val3);
}
void process_SRLI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_shamt(inst);
	u32 val3=val1 >> val2;
	save_reg(instr.Itype.rd,val3);
	printf("Final value %u\n",val3);
}

void process_SRAI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1=get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2=get_shamt(inst);
	u32 val3= ((s32)val1) >> val2;
	save_reg(instr.Itype.rd,val3);
	printf("SRA Final value %u\n",val3);
}

void process_ADD(u32 inst)                                                                                                            {
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = get_reg(instr.Rtype.rs2);
	printf("Value 2 %u\n",val2);
	u32 val3 = val1 + val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value of addition operation: %u\n",val3);
}

void process_SUB(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = get_reg(instr.Rtype.rs2);
	printf("Value 2 %u\n",val2);
	u32 val3 = val1 - val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value of subtraction operation: %u\n",val3);
}

void process_XOR(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = get_reg(instr.Rtype.rs2);
	printf("Value 2 %u\n",val2);
	u32 val3 = val1 ^ val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value of xor opeartion: %u\n",val3);
}

void process_OR(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = get_reg(instr.Rtype.rs2);
	printf("Value 2 %u\n",val2);
	u32 val3 = val1 | val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value of or opeartion: %u\n",val3);
}

void process_AND(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Rtype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = get_reg(instr.Rtype.rs2);
	printf("Value 2 %u\n",val2);
	u32 val3 = val1 & val2;
	save_reg(instr.Rtype.rd,val3);
	printf("Final value of and opeartion: %u\n",val3);
}


void process_ADDI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = signExtension((u32)instr.Itype.imm);
	printf("Value of immediate is %u\n",val2);
	u32 val3 = val1 + val2;
	save_reg(instr.Itype.rd,val3);
	printf("Final value of ADDI opeartion is %u\n",val3);
}

void process_XORI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = signExtension((u32)instr.Itype.imm);
	printf("Value of immediate is %u\n",val2);
	u32 val3 = val1 ^ val2;
	save_reg(instr.Itype.rd,val3);
	printf("Final value of XORI opeartion is %u\n",val3);
}

void process_ORI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = signExtension((u32)instr.Itype.imm);
	printf("Value of immediate is %u\n",val2);
	u32 val3 = val1 | val2;
	save_reg(instr.Itype.rd,val3);
	printf("Final value of ORI opeartion is %u\n",val3);
}

void process_ANDI(u32 inst)
{
	union riscv_inst32 instr = {inst};
	u32 val1 = get_reg(instr.Itype.rs1);
	printf("Value 1 %u\n",val1);
	u32 val2 = signExtension((u32)instr.Itype.imm);
	printf("Value of immediate is %u\n",val2);
	u32 val3 = val1 & val2;
	save_reg(instr.Itype.rd,val3);
	printf("Final value of ANDI opeartion is %u\n",val3);
}




