//Project was completed by Chandler Douglas, David Portz, and Jesse Alsing
//Completion date: 4 - 20 - 2017
//Commented accordingly by each individual member of the group.
#include "spimcore.h"

/* Pre-Condition: pass in A,B ALUControl, ALUresult, and *zero*/
/* Post-Condition: we set the value pointed to by ALUresult to the outcome of the given operation
set by ALUControl, we also set the value of Zero based on if the result is 0 and leave as 0 otherwise*/
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    //We initialize zero to zero then change it as needed
    *Zero = 0;
    //Here we just perform the operation based on ALUControl input
    if(ALUControl == 0)
        *ALUresult = A + B;
    else if(ALUControl == 1)
        *ALUresult = A - B;
    else if(ALUControl == 2)
    {
        if((signed)A < (signed)B)
            *ALUresult = 1;
        else
        {
            *ALUresult = 0;
            //Here the result is zero so we change zero to 1
            *Zero = 1;
        }
    }
    else if(ALUControl == 3)
    {
        if(A < B)
            *ALUresult = 1;
        else
        {
            *ALUresult = 0;
            //Here we change zero to one because the result is 1
            *Zero = 1;
        }
    }
    else if(ALUControl == 4)
        *ALUresult = A & B;
    else if(ALUControl == 5)
        *ALUresult = A | B;
    else if(ALUControl == 6)
        *ALUresult = B << 16;
    else
         *ALUresult = !A;

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //Check if the word is aligned update the instruction and return a 1
	if (((PC % 4) != 0) || (PC >> 2) > 0x10000)
	{
		return 1;
    }
    else
    {
        //Fetch instruction addressed by PC from Mem and write to instruction
        *instruction = Mem[PC >> 2];
        return 0;
    }
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //Temporary variables to get the upper bound of each instruction set.
    //Need for r1, r2, r3, funct, offset, and jsec;
    //Instructions are correctly partitioned by shifting the instruction variable
    //to the start of the instruction and anding the temporary variables to it.
    unsigned tempRegisters = 0x0000001F;
    unsigned tempFunct = 0x0000003F;
    unsigned tempOffset = 0x0000FFFF;
    unsigned tempJsec = 0x3FFFFFF;

    //Instructions [31-26]
    *op = instruction >> 26;

    //Instructions [25-21]
    *r1 = (instruction >> 21) & tempRegisters;

    //Instructions [20-16]
    *r2 = (instruction >> 16) & tempRegisters;

    //Instructions [15-11]
    *r3 = (instruction >> 11) & tempRegisters;

    //Instructions [5-0]
    *funct = instruction & tempFunct;

    //Instructions [15-0]
    *offset = instruction & tempOffset;

    //Instructions [25-0]
    *jsec = instruction & tempJsec;
}


/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
     switch(op)
    {
        //All R-Type cases
        case 0x00:
        {
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x07;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        }
        //Add immediate
        case 0x08:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x00;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        }
        //Load word
        case 0x23:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0x00;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        }
        //Store word
        case 0x2B:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x00;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;
        }
        //Load upper immediate
        case 0x0F:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x06;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        }
        //Branch on equal
        case 0x04:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x01;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        }
        //Set less than immediate
        case 0x0A:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x02;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        }
        //Set less than immediate unsigned
        case 0x0B:
        {
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x03;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        }
        //Jump
        case 0x02:
        {
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0x00;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        }
        default:
            return 1;
    }
    return 0;
}


/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Go to r1 offset index of Reg and save its value to data1
    *data1 = Reg[r1];

    // Go to r2 offset index of Reg and save its value to data2
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{
    //Temp variable is made to check if the leading bit is 1 or not.
    unsigned temp = offset;
    temp = temp >> 15;

    //If the leading bit is 1 and the number is negative, fill the top 16 bits with 1's
    //Done by ORing.
    if(temp == 1){
        *extended_value = (0b11111111111111110000000000000000 | offset);
    }

    //Else, fill the top 16 bits with 0
    //This doesn't actually change the value but it's nice to see visually what's happening.
    else{
        *extended_value = (0b00000000000000000000000000000000 | offset);
    }
}

/* Pre-Condition: passes in an unsigned data1,data2,extended_val,funct, char ALUOp, ALUSrc, Zero and unsigned ALUResult */
/* Post-Condition: returns 1 if a halt condition occurred or a 0 if a successful operation occurred */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    //set a temp variable to be changed when the instruction is an R-type
        char ALUControl = ALUOp;
    //this takes care of if  ALUSrc is 1 if we are to pass in extended value as data2.
	if (ALUSrc)
	{
		// this takes care of I types
		data2 = extended_value;
	}

	if(ALUOp < 7)
    {
        ALU(data1,data2,ALUOp,ALUresult,Zero);// sends I-type instructions to the ALU
    }
    //Sets ALUControl depending on the R-type command in Hexadecimal
	else if(ALUOp == 7)
    {
            if(funct == 0x20)//add
                ALUControl = 0;
            else if(funct == 0x22)//subtract
                ALUControl = 1;
            else if(funct == 0x2A)//SLT signed
                ALUControl = 2;
            else if(funct == 0x2B)//SLT unsigned
                ALUControl = 3;
            else if(funct == 0x24)//AND
                ALUControl = 4;
            else if(funct == 0x25)//OR
                ALUControl = 5;
            else
                return 1;//A halt condition has occurred
    }
    else if(ALUOp > 7)
        return 1;//A halt condition has occurred

	//pass in R-types to the ALU after setting the ALUControl
	ALU(data1, data2, ALUControl, ALUresult, Zero);

	// No halt condition encountered
	return 0;
}

/* Read / Write Memory */
/* 10 Points */
// Determine whether memory read or memory write operation
// Read the content of the memory location addressed by ALUresult to memdata.
// Write the value of data2 to the memory location addressed by ALUresult.
// Return 1 if a halt condition occurs; otherwise, return 0.
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // If ALUresult is not word aligned, exit immediately with a 1 (halt condition).
    //  mod 4 because memory is byte-addressed
	unsigned MemZero = 0x00000000;

	//If MemRead or MemWrite is asserted and word not aligned or out of bounds, halt
	//If we don't have this if statement, it is possible to encounter a false halt in the program.
	if ((MemRead | MemWrite) && ((ALUresult >> 2) < MemZero) | (ALUresult % 4 != 0 | (ALUresult >> 2) > 0xFFFF))
        return 1;

    // If we need to write to memory, write!
    // right shift by 2 because memory is byte-addressed: AKA divide by 4 to get byte
    if(MemWrite == 1)
    {
        Mem[ALUresult>>2] = data2;
    }
    // If we need to read from memory, read!
    // right shift by 2 because memory is byte-addressed: AKA divide by 4 to get byte
    if(MemRead == 1)
    {
        *memdata = Mem[ALUresult>>2];
    }

    // If we did not find a halt condition, return 0 to indicate all OK
    return 0;
}


/* Pre-Condition: pass in 2 addresses to registers and memdata, ALUResult, RegWrite, RegDst, MemtoReg, and the register */
/* Post-Condition: no  return but we set the memdata to the register given by RegDst or if no MemtoReg we set the ALUresult to the given register destination */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // we create a temp variable
    unsigned temp;
    // set to either r3 or r2 depending on if the Register destination is 0 or 1
    if (RegDst)
     		temp = r3;
     	else
     		temp = r2;
    //if we are supposed to register write we then look if we are going to address memory to a register
    if (RegWrite == 1)
    {
        if (MemtoReg == 1)
            Reg[temp] = memdata;
        else
            Reg[temp] = ALUresult;
        //if we do not have any memory to write we send the ALU result to the register.
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	 //Update to the PC
    *PC += 4;

    //Takes care of the case if jump = 0
    if(Jump == 0){
        //Branching and Zero is asserted
        //Calculate the new address of the PC using extended_value
        if((Branch && Zero) == 1)
            *PC += extended_value << 2;
    }

    //If jump doesn't equal 0 then it has to equal 1.
    //Calculate the PC based on a jump.
    else
    	*PC = (*PC & 0xF0000000) | (jsec << 2);
}
