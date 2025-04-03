
/*
 * Author: Ol' Jim
 * Editor: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// Already worked

#include "Instruction.h"

void lw_immd_assm(void)
{

    // Check if the opcode matches "LW"
    if (strcmp(OP_CODE, "LW") != 0)
    {
        // If the opcode doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Validate the types of parameters
    */

    // The third parameter (PARAM3) must be a register (source register Rs)
    if (PARAM3.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The first parameter (PARAM1) must be a register (destination register Rt)
    if (PARAM1.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The second parameter (PARAM2) must be an immediate value (offset)
    if (PARAM2.type != IMMEDIATE)
    {
        state = INVALID_IMMED;
        return;
    }

    /*
        Validate the values of parameters
    */

    // PARAM1 (Rt) must be a valid register number (0-31)
    if (PARAM1.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM3 (Rs) must be a valid register number (0-31)
    if (PARAM3.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM2 (offset) must be a valid signed 16-bit value (-32768 to 32767)
    if (PARAM2.value > 0x7FFF)
    {
        state = INVALID_IMMED;
        return;
    }

    /*
        Construct the binary instruction
    */

    // Set the opcode for LW (100011)
    setBits_str(31, "100011");

    // Set Rs (source register)
    setBits_num(25, PARAM3.value, 5);

    // Set Rt (destination register)
    setBits_num(20, PARAM1.value, 5);

    // Set the offset (immediate value)
    setBits_num(15, PARAM2.value, 16);

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void lw_immd_bin(void)
{

    // Check if the opcode bits match "LW"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "100011") != 0)
    {
        // If the opcode doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rs = getBits(25, 5);     // Source register Rs
    uint32_t Rt = getBits(20, 5);     // Destination register Rt
    uint32_t imm16 = getBits(15, 16); // Immediate value (offset)

    /*
        Set the instruction values
    */

    setOp("LW");                   // Set the operation to "LW"
    setParam(1, REGISTER, Rt);     // Set PARAM1 as the destination register Rt
    setParam(3, REGISTER, Rs);     // Set PARAM3 as the source register Rs
    setParam(2, IMMEDIATE, imm16); // Set PARAM2 as the immediate value (offset)

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
