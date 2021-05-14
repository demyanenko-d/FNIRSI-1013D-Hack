//----------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "armv5tl_thumb.h"

//----------------------------------------------------------------------------------------------------------------------------------

void ArmV5tlHandleThumb(PARMV5TL_CORE core)
{
  u_int16_t *memorypointer;

  //Instruction fetch for thumb state
  memorypointer = (u_int16_t *)ArmV5tlGetMemoryPointer(core, *core->program_counter, ARM_MEMORY_SHORT);

  //Check if a valid address is found
  if(memorypointer)
  {
    //get the current instruction
    core->thumb_instruction.instr = (u_int16_t)*memorypointer;
    
    //Decode based on the type bits
    switch(core->thumb_instruction.base.type)
    {
      case 0:
        //Check on instruction opcode
        if(core->thumb_instruction.base.op1 == 3)
        {
          //op1:3 ADD(3), SUB(3), ADD(1), MOV(2), SUB(1)
          ArmV5tlThumbDP0(core);
        }
        else
        {
          //LSL(1) op1:0, LSR(1) op1:1, ASR(1) op1:2
          ArmV5tlThumbShiftImmediate(core);
        }
        break;

      case 1:
        //MOV(1), CMP(1), ADD(2), SUB(2)
        ArmV5tlThumbDP1(core);
        break;

      case 2:
        //Check if data processing or load / store instructions
        if(core->thumb_instruction.base.op1 == 0)
        {
          //Separate the shift instructions
          if((core->thumb_instruction.base.op2 == 2) || (core->thumb_instruction.base.op2 == 3) || (core->thumb_instruction.base.op2 == 4) || (core->thumb_instruction.base.op2 == 7))
          {
            //LSL(2) op2:2, LSR(2) op2:3, ASR(2) op2:4, ROR op2:7
            ArmV5tlThumbShiftRegister(core);
          }
          //Get the other data processing functions except CMP(3), ADD(4), CPY and MOV(3)
          else if(core->thumb_instruction.base.op2 < 16)
          {
            //AND, EOR, ADC, SBC, TST, NEG, CMP(2), CMN, OR, BIC, MVN, MUL
            ArmV5tlThumbDP2(core);
          }
          //Filter out the branch instructions
          else if((core->thumb_instruction.base.op2 & 0x1C) == 0x1C)
          {
            //BLX(2), BX
            ArmV5tlThumbBranch2(core);
          }
          //The remainder are the special data processing functions
          else
          {
            //CMP(3), ADD(4), CPY and MOV(3)
            ArmV5tlThumbDP2S(core);
          }
        }
        //Filter out the load immediate indexed instruction LDR(3)
        else if(core->thumb_instruction.base.op1 == 1)
        {
          //LDR(3)
          ArmV5tlThumbLS2I(core);
        }
        else
        {
          //STR(2), STRH(2), STRB(2), LDR(2), LDRH(2), LDRB(2), LDRSB, LDRSH          
          ArmV5tlThumbLS2R(core);
        }
        break;

      case 3:
        //Load / store word / byte  immediate offset
        
        //             b l
        //STR(1)   011 0 0 iiiii nnn ddd
        //LDR(1)   011 0 1 iiiii nnn ddd
        //STRB(1)  011 1 0 iiiii nnn ddd
        //LDRB(1)  011 1 1 iiiii nnn ddd
        break;

      case 4:
        //Load / store short immediate offset
        //Load / store to / from stack
        
        //STR(3)   100 10 ddd iiiiiiii     (sp is used)
        //LDR(4)   100 11 ddd iiiiiiii     (sp is used)
        
        //STRH(1)  100 00 iiiii nnn ddd
        //LDRH(1)  100 01 iiiii nnn ddd
        
        break;

      case 5:
        //Add to sp or pc
        //Miscellaneous
        
        //ADD(5)   101 00 ddd iiiiiiii
        //ADD(6)   101 01 ddd iiiiiiii
        
        //ADD(7)   101 10 0000 iiiiiii
        
        //BKPT     101 11 110 iiiiiiii
        
        //CPS      101 10 1100 11 m 0 a i f
        
        //POP      101 11 10 r llllllll
        //PUSH     101 10 10 r llllllll
        
        //REV      101 11 01000 nnn ddd
        //REV16    101 11 01001 nnn ddd
        //REVSH    101 11 01011 nnn ddd
        
        //SETEND   101 10 11001 01 e zzz
        
        //SUB(4)   101 10 0001 iiiiiii
        
        //SXTB     101 10 01001 mmm ddd
        //SXTH     101 10 01000 mmm ddd
        //UXTB     101 10 01011 mmm ddd
        //UXTH     101 10 01010 mmm ddd
        
        break;

      case 6:
        //Load / store multiple
        //Conditional branch
        //Undefined instruction
        //Software interrupt
        
        
        //STMIA 110 0 0 nnn llllllll
        //LDMIA 110 0 1 nnn llllllll

        //B(1)  110 1 cccc iiiiiiii
        
        //SWI   110 1 1111 iiiiiiii
        
        //UI    110 1 1110 xxxxxxxx
        
        break;

      case 7:
        //Unconditional branch
        //BLX suffix
        //Undefined instruction
        //BL / BLX prefix
        //BL suffix
        
        //B(2)    111 00 iiiiiiiiiii
        
        //BLX(1)  111 01 iiiiiiiiiii
        //BL      111 10 iiiiiiiiiii
        //BL      111 11 iiiiiiiiiii
        
        break;
    }
  }
  else
  {
    //Some exception needs to be generated here. Undefined Instruction most likely
    ArmV5tlUndefinedInstruction(core);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------
//Immediate shift
void ArmV5tlThumbShiftImmediate(PARMV5TL_CORE core)
{
  u_int32_t sa = core->thumb_instruction.shift0.sa;
  u_int32_t vm = *core->registers[core->current_bank][core->thumb_instruction.shift0.rm];
  u_int32_t type;
  
  
  switch(core->thumb_instruction.shift0.op1)
  {
    case 0:
      type = ARM_SHIFT_MODE_LSL;
      break;
      
    case 1:
      type = ARM_SHIFT_MODE_LSR;
      
      //Check if intended shift is 32. Shift amount in instruction is 0
      if(sa == 0)
        sa = 32;
      break;
      
    case 2:
      type = ARM_SHIFT_MODE_ASR;
      
      //Check if intended shift is 32. Shift amount in instruction is 0
      if(sa == 0)
        sa = 32;
      break;
  }
  
  //Do the actual shifting
  ArmV5tlThumbShift(core, type, sa , vm);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Register shift
void ArmV5tlThumbShiftRegister(PARMV5TL_CORE core)
{
  u_int32_t sa = *core->registers[core->current_bank][core->thumb_instruction.shift2.rs];
  u_int32_t vm = *core->registers[core->current_bank][core->thumb_instruction.shift2.rd];
  u_int32_t type;
  
  //LSL(2) op2:2, LSR(2) op2:3, ASR(2) op2:4, ROR op2:7
  //Other values are filtered out in the decode stage
  switch(core->thumb_instruction.shift2.op2)
  {
    case 2:
      type = ARM_SHIFT_MODE_LSL;
      break;
      
    case 3:
      type = ARM_SHIFT_MODE_LSR;
      break;
      
    case 4:
      type = ARM_SHIFT_MODE_ASR;
      break;
      
    case 7:
      type = ARM_SHIFT_MODE_ROR;
      break;
  }
  
  //Do the actual shifting
  ArmV5tlThumbShift(core, type, sa, vm);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Shift
void ArmV5tlThumbShift(PARMV5TL_CORE core, u_int32_t type, u_int32_t sa, u_int32_t vm)
{
  u_int32_t c = core->status->flags.C;
  
  //Take action based on the shift type
  switch(type)
  {
    case ARM_SHIFT_MODE_LSL:
      if((sa > 0) && (sa < 32))
      {
        //When the shift is less then 32 the carry is the last bit shifted out and vm is shifted sa times
        c = (vm >> (32 - sa)) & 1;
        vm <<= sa;
      }
      else if(sa == 32)
      {
        //When the shift is 32 the carry is bit0 of vm and vm is set to zero
        c = vm & 1;
        vm = 0;
      }
      else if(sa > 32)
      {
        //when shifting is more then 32 both carry and vm are set to zero
        c = 0;
        vm = 0;
      }
      break;
      
    case ARM_SHIFT_MODE_LSR:
      if((sa > 0) && (sa < 32))
      {
        //When the shift is less then 32 the carry is the last bit shifted out and vm is shifted sa times
        c = (vm >> (sa - 1)) & 1;
        vm >>= sa;
      }
      else if(sa == 32)
      {
        //When the shift is 32 the carry is bit0 of vm and vm is set to zero
        c = vm >> 31;
        vm = 0;
      }
      else
      {
        //when shifting is more then 32 both carry and vm are set to zero
        c = 0;
        vm = 0;
      }
      break;
      
    case ARM_SHIFT_MODE_ASR:
      if((sa > 0) && (sa < 32))
      {
        //When the shift is less then 32 the carry is the last bit shifted out and vm is shifted sa times
        c = (vm >> (sa - 1)) & 1;
        
        //When positive normal shifting is ok
        if((vm & 0x80000000) == 0)
        {
          vm >>= sa;
        }
        else
        {
          //For negative numbers invert, shift and invert back
          vm = ~(~vm >> sa);
        }
      }
      else if(sa >= 32)
      {
        //When the shift is 32 or more the carry is bit31
        c = vm >> 31;
        
        //Based on the sign bit of the data the result is either 0 or 0xFFFFFFFF
        if(c == 0)
        {
          vm = 0;
        }
        else
        {
          vm = 0xFFFFFFFF;
        }
      }
      break;
      
    case ARM_SHIFT_MODE_ROR:
      if((sa > 0) && ((sa & 0x1F) == 0))
      {
        //When only the lowest 5 bits are zero the carry is bit31
        c = vm >> 31;
      }
      else
      {
        //Make sure upper bits of the number of rotates are cleared
        sa &= 0x1F;
        
        //Get the carry
        c = (vm >> (sa - 1)) & 1;
        
        //rotate the bits
        vm = (vm >> sa) | (vm << (32 - sa));
      }
      break;
  }
  
  //Store back to rd (For rd both shift0 and shift2 types are the same)
  *core->registers[core->current_bank][core->thumb_instruction.shift0.rd] = vm;
  
  //Update the negative bit
  core->status->flags.N = vm >> 31;

  //Update the zero bit
  core->status->flags.Z = (vm == 0);
  
  //Update the carry
  core->status->flags.C = c;
}

//----------------------------------------------------------------------------------------------------------------------------------
//Data processing type 0
void ArmV5tlThumbDP0(PARMV5TL_CORE core)
{
  //Get the input data. For rd and vn there is no difference between dpi0 and dpr0
  u_int32_t rd = core->thumb_instruction.dpi0.rd;
  u_int32_t vm;
  u_int32_t vn = *core->registers[core->current_bank][core->thumb_instruction.dpr0.rn];
  u_int32_t type;
  
  //For op2 both dpr0 and dpi0 are the same
  switch(core->thumb_instruction.dpr0.op2)
  {
    //ADD(3)
    case 0:
      //Use the indicated register for the second operand
      vm = *core->registers[core->current_bank][core->thumb_instruction.dpr0.rm];
      type = ARM_OPCODE_ADD;
      break;

    //SUB(3)
    case 1:
      //Use the indicated register for the second operand
      vm = *core->registers[core->current_bank][core->thumb_instruction.dpr0.rm];
      type = ARM_OPCODE_SUB;
      break;
    
    //ADD(1), MOV(2)
    case 2:
      //Check if add or mov instruction
      if(core->thumb_instruction.dpi0.im)
      {
        //For a non zero immediate value it is an add
        //Use the immediate value for the second operand
        vm = core->thumb_instruction.dpi0.im;
        type = ARM_OPCODE_ADD;
      }
      else
      {
        //For a zero immediate value it is move
        //Use the data from the n register as operand
        vm = vn;
        
        //Signal that for this move the carry and overflow flag need to be cleared
        type = ARM_OPCODE_MOV | ARM_OPCODE_THUMB_CLR_CV;
      }
      break;

    //SUB(1)
    case 3:
      //Use the immediate value for the second operand
      vm = core->thumb_instruction.dpi0.im;
      type = ARM_OPCODE_SUB;
      break;
  }
  
  //Go and do the actual processing
  ArmV5tlThumbDP(core, type, rd, vn, vm);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Data processing type 1
void ArmV5tlThumbDP1(PARMV5TL_CORE core)
{
  //Get the input data.
  u_int32_t rd = core->thumb_instruction.dp1.rd;
  u_int32_t vm = core->thumb_instruction.dp1.im;
  u_int32_t vn = *core->registers[core->current_bank][core->thumb_instruction.dp1.rd];
  u_int32_t type;
  
  //For type 1 the op1 is the select for the type of function to perform
  switch(core->thumb_instruction.dp1.op1)
  {
    //MOV(1)
    case 0:
      type = ARM_OPCODE_MOV;
      break;

    //CMP(1)
    case 1:
      type = ARM_OPCODE_CMP;
      break;
    
    //ADD(2)
    case 2:
      type = ARM_OPCODE_ADD;
      break;

    //SUB(2)
    case 3:
      type = ARM_OPCODE_SUB;
      break;
  }
  
  //Go and do the actual processing
  ArmV5tlThumbDP(core, type, rd, vn, vm);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Data processing type 2
void ArmV5tlThumbDP2(PARMV5TL_CORE core)
{
  //Get the input data.
  u_int32_t rd = core->thumb_instruction.dp2.rd;
  u_int32_t vm = *core->registers[core->current_bank][core->thumb_instruction.dp2.rm];
  u_int32_t vn = *core->registers[core->current_bank][core->thumb_instruction.dp2.rd];
  u_int32_t type;
  
  //For type 2 dp2 op2 tells which function to perform. Some have already been filtered out in the decoding process.
  switch(core->thumb_instruction.dp2.op2)
  {
    //AND
    case 0:
      type = ARM_OPCODE_AND;
      break;

    //EOR
    case 1:
      type = ARM_OPCODE_EOR;
      break;
    
    //ADC
    case 5:
      type = ARM_OPCODE_ADC;
      break;

    //SBC
    case 6:
      type = ARM_OPCODE_SBC;
      break;
      
    //TST
    case 8:
      type = ARM_OPCODE_TST;
      break;
      
    //NEG
    case 9:
      type = ARM_OPCODE_THUMB_NEG;
      break;
      
    //CMP(2)
    case 10:
      type = ARM_OPCODE_CMP;
      break;

    //CMN
    case 11:
      type = ARM_OPCODE_CMN;
      break;

    //ORR
    case 12:
      type = ARM_OPCODE_ORR;
      break;

    //MUL
    case 13:
      type = ARM_OPCODE_THUMB_MUL;
      break;

    //BIC
    case 14:
      type = ARM_OPCODE_BIC;
      break;

    //MVN
    case 15:
      type = ARM_OPCODE_MVN;
      break;
  }
  
  //Go and do the actual processing
  ArmV5tlThumbDP(core, type, rd, vn, vm);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Data processing type 2 special
void ArmV5tlThumbDP2S(PARMV5TL_CORE core)
{
 //Get the input data. rd is a combination of the h (high) bit and the three rd bits
  u_int32_t rd = core->thumb_instruction.dp2s.rd | (core->thumb_instruction.dp2s.h << 3);
  u_int32_t vm = *core->registers[core->current_bank][core->thumb_instruction.dp2s.rm];
  u_int32_t vn = *core->registers[core->current_bank][rd];
  u_int32_t type;
  
  //Not sure about this. Manual does not give enogh info
  //Amend the values when r15 (pc) is used
  if(core->thumb_instruction.dp2s.rm == 15)
  {
    vm += 4;
  }

  //Same for the first operand register
  if(rd == 15)
  {
    vn += 4;
  }
  
  //For type 2 dp2s op2 tells which function to perform. Most have already been filtered out in the decoding process.
  switch(core->thumb_instruction.dp2s.op2)
  {
    //ADD(4)
    case 4:
      type = ARM_OPCODE_ADD | ARM_OPCODE_THUMB_NO_FLAGS;
      break;

    //CMP(3)
    case 5:
      type = ARM_OPCODE_CMP;
      break;

    //CPY, MOV(3)
    case 6:
      type = ARM_OPCODE_MOV | ARM_OPCODE_THUMB_NO_FLAGS;
      break;
  }  
}

//----------------------------------------------------------------------------------------------------------------------------------
//Actual data processing handling
void ArmV5tlThumbDP(PARMV5TL_CORE core, u_int32_t type, u_int32_t  rd, u_int32_t vn, u_int32_t vm)
{
  u_int64_t vd;
  u_int32_t update = 1;
  u_int32_t docandv = ARM_FLAGS_UPDATE_CV_NO;
  
  //Check if NEG
  if(type & ARM_OPCODE_THUMB_NEG)
  {
    vd = 0 - (int64_t)vm;
  }
  //If not check if MUL
  else if(type & ARM_OPCODE_THUMB_MUL)
  {
    vd = (int32_t)vm * (int32_t)vn;
  }
  //If neither do the switch on type
  else
  {
    //Perform the correct action based on the type
    switch(type & ARM_THUMB_TYPE_MASK)
    {
      case ARM_OPCODE_AND:
        vd = vn & vm;
        break;

      case ARM_OPCODE_EOR:
        vd = vn ^ vm;
        break;

      case ARM_OPCODE_SUB:
        vd = (int64_t)vn - (int64_t)vm;
        break;

      case ARM_OPCODE_RSB:
        vd = (int64_t)vm - (int64_t)vn;
        break;

      case ARM_OPCODE_ADD:
        vd = (int64_t)vn + (int64_t)vm;
        break;

      case ARM_OPCODE_ADC:
        vd = (int64_t)vn + (int64_t)vm + core->status->flags.C;
        break;

      case ARM_OPCODE_SBC:
        vd = (int64_t)vn - (int64_t)vm - (core->status->flags.C ^ 1);
        break;

      case ARM_OPCODE_RSC:
        vd = (int64_t)vm - (int64_t)vn - (core->status->flags.C ^ 1);
        break;

      case ARM_OPCODE_TST:
        vd = vn & vm;
        break;

      case ARM_OPCODE_TEQ:
        vd = vn ^ vm;
        break;

      case ARM_OPCODE_CMP:
        vd = (int64_t)vn - (int64_t)vm;
        break;

      case ARM_OPCODE_CMN:
        vd = (int64_t)vn + (int64_t)vm;
        break;

      case ARM_OPCODE_ORR:
        vd = vn | vm;
        break;

      case ARM_OPCODE_MOV:
        vd = vm;
        break;

      case ARM_OPCODE_BIC:
        vd = vn & ~vm;
        break;

      case ARM_OPCODE_MVN:
        vd = ~vm;
        break;
    }
  }
  
  //Clear the update rd flag for the types that do not save the result
  switch(type & ARM_THUMB_TYPE_MASK)
  {      
    case ARM_OPCODE_TST:
    case ARM_OPCODE_TEQ:
    case ARM_OPCODE_CMP:
    case ARM_OPCODE_CMN:
      update = 0;
      break;
  }
  
  //Check on extra flags in the type
  if(type & ARM_OPCODE_THUMB_CLR_CV)
  {
    //If clear carry and overflow specified set the indicator accordingly
    docandv = ARM_FLAGS_UPDATE_CV_CLR;
  }
  //Check if NEG function requested
  else if(type & ARM_OPCODE_THUMB_NEG)
  {
    docandv = ARM_FLAGS_UPDATE_NBV;
  }
  //If not MUL do the switch on type
  else if((type & ARM_OPCODE_THUMB_MUL) == 0)
  {
    //Set the flag change mode based on the requested type
    switch(type & ARM_THUMB_TYPE_MASK)
    {
      case ARM_OPCODE_ADD:
      case ARM_OPCODE_ADC:
      case ARM_OPCODE_CMN:
        docandv = ARM_FLAGS_UPDATE_CV;
        break;

      case ARM_OPCODE_SUB:
      case ARM_OPCODE_RSB:
      case ARM_OPCODE_SBC:
      case ARM_OPCODE_RSC:
      case ARM_OPCODE_CMP:
        docandv = ARM_FLAGS_UPDATE_NBV;
        break;
    }
  }
  
  //Check if flags can be updated (Some ADD and MOV instructions do not update the flags)
  if((type & ARM_OPCODE_THUMB_NO_FLAGS) == 0)
  {
    //Update the negative bit
    core->status->flags.N = vd >> 31;

    //Update the zero bit
    core->status->flags.Z = (vd == 0);

    //Check if carry and overflow need to be updated with arithmetic result
    if(docandv == ARM_FLAGS_UPDATE_CV_CLR)
    {
      core->status->flags.V = 0;
      core->status->flags.C = 0;
    }
    else if(docandv != ARM_FLAGS_UPDATE_CV_NO)
    {
      //Update the overflow bit
      core->status->flags.V = (((vn & 0x80000000) == (vm & 0x80000000)) && (vn & 0x80000000) != (vd & 0x80000000));

      //Handle the carry according to type of arithmetic
      if(docandv == ARM_FLAGS_UPDATE_CV)
      {
        //Carry from addition
        core->status->flags.C = vd >> 32;
      }
      else
      {
        //Not borrow from subtraction
        core->status->flags.C = (vd <= 0xFFFFFFFF);
      }
    }
  }
  
  //Check if destination register needs to be updated
  if(update)
  {
    //Write the result back as a singed 32 bit integer
    *core->registers[core->current_bank][rd] = (int32_t)vd;
    
    //Check if program counter used as target
    if(rd == 15)
    {
      //Signal no increment if so
      core->pcincrvalue = 0;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------
//Thumb load and store handling for type 2 immediate indexed based instructions
void ArmV5tlThumbLS2I(PARMV5TL_CORE core)
{
 //Get the input data
  u_int32_t rd = core->thumb_instruction.ls2i.rd;
  u_int32_t vm = core->thumb_instruction.ls2i.im * 4;
  u_int32_t vn = (*core->program_counter + 4) & 0xFFFFFFFC;
  u_int32_t type;
  u_int32_t address = vn + vm;

  //Signal loading of a word
  type = ARM_THUMB_SIZE_WORD | ARM_THUMB_LOAD_FLAG;
  
  //Go and do the actual load or store
  ArmV5tlThumbLS(core, type, rd, address);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Thumb load and store handling for type 2 register based instructions
void ArmV5tlThumbLS2R(PARMV5TL_CORE core)
{
 //Get the input data
  u_int32_t rd = core->thumb_instruction.ls2r.rd;
  u_int32_t vm = *core->registers[core->current_bank][core->thumb_instruction.ls2r.rm];
  u_int32_t vn = *core->registers[core->current_bank][core->thumb_instruction.ls2r.rn];
  u_int32_t type;
  u_int32_t address = vn + vm;
  
  //Check if sign extend instructions
  if(core->thumb_instruction.ls2r.op2 == 3)
  {
    //Check on which size the instruction needs to work
    if(core->thumb_instruction.ls2r.op1 == 2)
    {
      //LDRSB
      type = ARM_THUMB_SIZE_BYTE;
    }
    else
    {
      //LDRSH
      type = ARM_THUMB_SIZE_SHORT;
    }
    
    //Signal load and sign extend
    type |= (ARM_THUMB_LOAD_FLAG | ARM_THUMB_SIGN_EXTEND);
  }
  //Normal load store instructions
  else
  {
    //Set the data size in the type
    type = core->thumb_instruction.ls2r.op2;
    
    //Check if load instructions
    if(core->thumb_instruction.ls2r.op1 == 3)
    {
      //Signal load functionality
      type |= ARM_THUMB_LOAD_FLAG;
    }
  }
  
  //Go and do the actual load or store
  ArmV5tlThumbLS(core, type, rd, address);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Thumb load and store instruction handling
void ArmV5tlThumbLS(PARMV5TL_CORE core, u_int32_t type, u_int32_t rd, u_int32_t address)
{
  void *memory;
  
  //Handle data based on the size of it
  switch(type & ARM_THUMB_SIZE_MASK)
  {
    case ARM_THUMB_SIZE_BYTE:
      //Byte access so get a pointer to the given address
      memory = ArmV5tlGetMemoryPointer(core, address, ARM_MEMORY_BYTE);

      //Check if the address was valid
      if(memory)
      {
        //Check if a load or a store is requested
        if(type & ARM_THUMB_LOAD_FLAG)
        {
          //Load from memory address
          *core->registers[core->current_bank][rd] = *(u_int8_t *)memory;
          
          //Check if sign extend needed
          if((type & ARM_THUMB_SIGN_EXTEND) && (*core->registers[core->current_bank][rd] & 0x80))
          {
            *core->registers[core->current_bank][rd] |= 0xFFFFFF00;
          }
        }
        else
        {
          //Store to memory address
          *(u_int8_t *)memory = (u_int8_t)*core->registers[core->current_bank][rd];
        }
      }
      break;
      
    case ARM_THUMB_SIZE_SHORT:
      //Short access so get a pointer to the given address
      memory = ArmV5tlGetMemoryPointer(core, address, ARM_MEMORY_SHORT);

      //Check if the address was valid
      if(memory)
      {
        //Check if a load or a store is requested
        if(type & ARM_THUMB_LOAD_FLAG)
        {
          //Load from memory address
          *core->registers[core->current_bank][rd] = *(u_int16_t *)memory;
          
          //Check if sign extend needed
          if((type & ARM_THUMB_SIGN_EXTEND) && (*core->registers[core->current_bank][rd] & 0x8000))
          {
            *core->registers[core->current_bank][rd] |= 0xFFFF0000;
          }
        }
        else
        {
          //Store to memory address
          *(u_int16_t *)memory = (u_int16_t)*core->registers[core->current_bank][rd];
        }
      }
      break;
      
    case ARM_THUMB_SIZE_WORD:
      //Word access so get a pointer to the given address
      memory = ArmV5tlGetMemoryPointer(core, address, ARM_MEMORY_WORD);
    
      //Check if the address is valid
      if(memory)
      {
        //Check if a load or a store is requested
        if(type & ARM_THUMB_LOAD_FLAG)
        {
          //Load from memory address
          *core->registers[core->current_bank][rd] = *(u_int32_t *)memory;
        }
        else
        {
          //Store to memory address
          *(u_int32_t *)memory = *core->registers[core->current_bank][rd];
        }
      }
      break;
  }
  
  //Check if program counter used as target
  if((type & ARM_THUMB_LOAD_FLAG) && (rd == 15))
  {
    //Signal no increment if so
    core->pcincrvalue = 0;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------
//Type 2 instructions branch handling
void ArmV5tlThumbBranch2(PARMV5TL_CORE core)
{
  u_int32_t vm = *core->registers[core->current_bank][core->thumb_instruction.b2.rm];
  
  //When the program counter is used the value needs to be plus 4
  if(core->thumb_instruction.b2.rm == 15)
  {
    vm += 4;
  }
  
  //Bit 0 of the target address given in rm is the T bit for the next instruction
  core->status->flags.T = vm & 1;
  
  //Check if BLX instruction
  if(core->thumb_instruction.b2.op2 == 15)
  {
    //Set the link register for the BLX instruction to point to the next thumb instruction
    *core->registers[core->current_bank][14] = (*core->program_counter + 2) | 1;
  }

  //Update the program counter with the target address without the thumb indicator bit
  *core->program_counter = vm & 0xFFFFFFFE;
  
  //Signal no increment of the pc
  core->pcincrvalue = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------
//Actual branch handling
void ArmV5tlThumbBranch(PARMV5TL_CORE core, u_int32_t type, u_int32_t address)
{
  
}
