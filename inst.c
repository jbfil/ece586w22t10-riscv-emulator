#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "memory.h"


void inst_type (u32 inst);
void Rtype_inst(u32 inst);
void Itype_inst(u32 inst);
void ltype_instr(u32 instr);
void stype_instr(u32 instr);

u08  get_memb(u16 addr);
u32 get_mem(u16 addr);
u16 get_memh(u16 addr);
s32 get_regsign(u32 addr)
void save_mword(u16 addr, u32 reg_val);
void save_mhw(u16 addr, u16 reg_hval);
void save_mbyte(u16 addr,u08 reg_bval);

s32 get_reg(u32 addr);
void save_reg(u32 addr,u32 value);
s32 signExtension(u32 inst);


int main_xx()
{
	

return 0;
};



void  inst_type (u32 inst)
 {
     union riscv_inst32 inst32 = { inst };
	//u08  opcode = extract_opcode(inst);

	switch (inst32.any.opcode){
	 /*   case 0b0110011:  Rtype_inst(inst);   //  r type
			        break;
		case 0b00010011:  Itype_inst(inst);  // immediate type
			        break;	
					*/  
				      
	case 0b00000011:  ltype_instr(inst);  // load type
			        break;

	case 0b00010011:  Itype_inst(inst);  // immediate type
			        break;
	case 0b00100011 :  stype_instr(inst);  // s type
			        	break;
	case 0b00110011:  Rtype_inst(inst);   //  r type
			        break;
	case 0b00110111: process_LUI(inst);   //  lui
                                break;
        case 0b00010111:  process_AUIPC(inst);  //  auipc
                                break;
        case 0b1101111: process_JAL(inst);   //  lui
                                break;
        case 0b1100111:  process_JALR(inst);  //  auipc
                                break;
			        /*
case 0b01100011:   btype_inst(inst); // branch type
			break;
case 0b01110011:  e_inst(inst);  //ebreak and ecall
			break;
*/		
default:  printf("error :  opcode not supported\n");
}

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
			case 0b011: process_SLTIU(inst);
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
	
	switch(loadInst.Itype.funct3) {
	case 0b000:   val_byte = get_memb(effective_addr);  // performing lb operations
			if((((val_byte & (1<<7)) >> 7 ) == 1) )
			{
		  	      val_word = 0xffffff00 | val_byte;
			}
		else {
			val_word = (u32) (val_byte);
			}	

			save_reg(loadInst.Itype.rd,val_word);
			break;

	case 0b001:   val_hw = get_memh(effective_addr);  // performing lh
			if((((val_hw & (1<<15)) >> 15 ) == 1)) 
			{
		        	val_word = 0xffff0000 | val_hw;
			}
			else {
				val_word = (u32) (val_hw);
			}

			save_reg(loadInst.Itype.rd,val_word);
			break;

	case 0b010:    val_word = get_mem(effective_addr); // performing lw
				
		
			save_reg(loadInst.Itype.rd,val_word);
			break;

	case 0b100:    	val_byte = get_memb(effective_addr); //performing lbu
			val_word = (u32) (val_byte);
			save_reg(loadInst.Itype.rd,val_word);
			break;


	case 0b101:	val_hw = get_memh(effective_addr); // performing lhu
			val_word = (u32) (val_hw);
			save_reg(loadInst.Itype.rd,val_word);
			break;

	default: 	printf("not a valid load type");
				break;
	}
	

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
   
	
    switch(storeInst.Stype.funct3) 
	{

   	case 0b000:	save_mbyte(effective_addr,reg_bval); //byte to mem
					break; 
	case 0b001:	save_mhw(effective_addr,reg_hval); //hw to mem
					break; 
	case 0b010: 	save_mword(effective_addr,reg_val);  //word to mem
					break;
	default:    	printf("store type not supported");
					break;
	}
}
	
	



s32 get_regsign(u32 addr) // 5 bit value
{
	
	if(addr<32)
{
		return regs[addr];
	}
		
	else
		printf("Register's address invalid");
		return 0;
}




u32 get_reg(u32 addr) // 5 bit value
{
	
	if(addr<32)
{
		return regs[addr];
	}
		
	else
		printf("Register's address invalid");
		return 0;
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
	if((addr & 0x1) == 0)
return mem.hwords[addr>>1];
else 
	{
		printf("Error: address is not half word alligned.");
		return 0;

}

}

u32 get_mem(u16 addr) {
		if((addr & 0x3) == 0)
			return mem.words[addr>>2];
else {
		
		printf("Error: address is not word alligned.");
		return 0;
}

}

u08  get_memb(u16 addr) {
	
	return mem.bytes[addr];

}

u32 get_shamt(u32 inst)
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
u32 val3=((s32) val1) >> val2;//Shifting a signed value, it behaves as arithmetic shift operator
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
u32 val2=signExtension(instr.Itype.imm);
u32 val3=((s32) val1 <(s32) val2)?(u32) 1:(u32)0;
save_reg(instr.Itype.rd,val3);
printf("Final value %u\n",val3);
}

void process_SLTIU(u32 inst)
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


void process_ADD(u32 inst)
{
union riscv_inst32 instr = {inst};
s32 val1 = get_regsign(instr.Rtype.rs1);
printf("Value 1 %d\n",val1);
s32 val2 = get_regsign(instr.Rtype.rs2);
printf("Value 2 %d\n",val2);
s32 val3 = val1 + val2;
save_reg(instr.Rtype.rd,val3);
printf("Final value of addition operation: %d\n",val3);
}

void process_SUB(u32 inst)
{
union riscv_inst32 instr = {inst};
s32 val1 = get_regsign(instr.Rtype.rs1);
printf("Value 1 %d\n",val1);
s32 val2 = get_regsign(instr.Rtype.rs2);
printf("Value 2 %d\n",val2);
s32 val3 = val1 - val2;
save_reg(instr.Rtype.rd,val3);
printf("Final value of subtraction operation: %d\n",val3);
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
s32 val1 =get_regsign(instr.Itype.rs1);
printf("Value 1 %d\n",val1);
s32 val2 = signExtension((s32)instr.Itype.imm);
printf("Value of immediate is %d\n",val2);
s32 val3 = val1 + val2;
save_reg(instr.Itype.rd,val3);
printf("Final value of ADDI opeartion is %d\n",val3);
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
