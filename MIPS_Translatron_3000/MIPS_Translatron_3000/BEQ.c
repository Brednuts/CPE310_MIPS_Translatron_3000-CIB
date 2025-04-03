
/*
 * Author: Ol' Jim
 * Editor: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// What was Changed: The rt and rs registers were flipped in the binary and machine code

#include "Instruction.h"

void beq_immd_assm(void)
{
    // Check if the opcode matches "BEQ"
    // strcmp(string1, string2) returns 0 if they match
    if (strcmp(OP_CODE, "BEQ") != 0)
    {
        // If the opcode doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Validate the types of parameters
    */

    // The first parameter (PARAM1) must be a register (source register Rs)
    if (PARAM1.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The second parameter (PARAM2) must be a register (source register Rt)
    if (PARAM2.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The third parameter (PARAM3) must be an immediate value (offset)
    if (PARAM3.type != IMMEDIATE)
    {
        state = INVALID_PARAM;
        return;
    }

    /*
        Validate the values of parameters
    */

    // PARAM1 (Rs) must be a valid register number (0-31)
    if (PARAM1.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM2 (Rt) must be a valid register number (0-31)
    if (PARAM2.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM3 (offset) must be a valid 16-bit value (0-0xFFFF)
    if (PARAM3.value > 0xFFFF)
    {
        state = INVALID_IMMED;
        return;
    }

    /*
        Construct the binary instruction
    */

    // Set the opcode for BEQ (000100)
    setBits_str(31, "000100");

    // Set Rs (source register)
    setBits_num(25, PARAM1.value, 5); // CHANGED from PARAM2 to PARAM1

    // Set Rt (source register)
    setBits_num(20, PARAM2.value, 5); // CHANGED from PARAM1 to PARAM2

    // Set the offset (immediate value)
    setBits_num(15, PARAM3.value, 16);

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void beq_immd_bin(void)
{

    // Check if the opcode bits match "BEQ"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    // Any 'x' in the bit string will be ignored
    if (checkBits(31, "000100") != 0) // CHANGED "001000" to "000100" for checkBits(31, "001000")
    {
        state = WRONG_COMMAND;
        return;
    }

    // If the opcode bits match, decode the binary instruction

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rs = getBits(25, 5);      // Source register Rs
    uint32_t Rt = getBits(20, 5);      // Source register Rt
    uint32_t offset = getBits(15, 16); // Immediate value (offset)

    /*
        Set the instruction values
    */

    setOp("BEQ");                   // Set the operation to "BEQ"
    setParam(1, REGISTER, Rs);      // Set PARAM1 as the source register Rs && CHANGED from rt to rs
    setParam(2, REGISTER, Rt);      // Set PARAM2 as the source register Rt && CHANGED from rs to rt
    setParam(3, IMMEDIATE, offset); // Set PARAM3 as the immediate value (offset)

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
