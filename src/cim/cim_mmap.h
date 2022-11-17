#ifndef CIM_MMAP_H_INCLUDED
#define CIM_MMAP_H_INCLUDED

#include "cim/cim_ioctl.h"

namespace gem5
{

// Reserved addresses
#define CIM_ADDR_EXECUTE 0x0 // Execute simulator
#define CIM_ADDR_INSTR_MEM 0x1 // Load instruction into instruction memory
#define CIM_ADDR_OPCODE 0x2 // Address for instruction opcode
#define CIM_ADDR_OP1 0x3 // Operand 1
#define CIM_ADDR_OP2 0x4 // Operand 2
#define CIM_ADDR_OP3 0x5 // etc..
#define CIM_ADDR_OP4 0x6
#define CIM_ADDR_OP5 0x7
#define CIM_ADDR_OP6 0x8
#define CIM_ADDR_OP7 0x9
#define CIM_ADDR_OP8 0xA
#define CIM_ADDR_OP9 0xB
#define CIM_ADDR_WD_MEM 0xC
#define CIM_ADDR_RD_MEM CIM_ADDR_WD_MEM + 0x200
#define CIM_ADDR_OUTPUT_MEM CIM_ADDR_RD_MEM + 0x200

// Opcodes for macro-instructions
#define CIM_OP_STORE 0
#define CIM_OP_MMM 1

} // namespace gem5

#endif