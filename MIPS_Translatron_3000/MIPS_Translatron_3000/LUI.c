/*
 * Author: Ol' Jim
 * Editor: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// What was Changed: There were one too many registers

#include "Instruction.h"

void lui_immd_assm(void)
{

    // Check if the opcode matches "LUI"
    if (strcmp(OP_CODE, "LUI") != 0)
    {
        // If the opcode doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Validate the types of parameters
    */

    // The first parameter (PARAM1) must be a register (destination register Rt)
    if (PARAM1.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The second parameter (PARAM2) must be an immediate value
    if (PARAM2.type != IMMEDIATE)
    {
        state = INVALID_PARAM;
        return;
    }

    // CHANGED: Removed the third parameter/register

    /*
        Validate the values of parameters
    */

    // PARAM1 (Rt) must be a valid register number (0-31)
    if (PARAM1.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM2 (immediate) must be a valid 16-bit value (0-0xFFFF)
    if (PARAM2.value > 0xFFFF)
    {
        state = INVALID_IMMED;
        return;
    }

    // CHANGED: Removed the third parameter/register

    /*
        Construct the binary instruction
    */

    // Set the opcode for LUI (001111)
    setBits_str(31, "001111");

    // Set the unused Rs field to 0
    setBits_num(25, 0, 5);

    // Set Rt (destination register)
    setBits_num(20, PARAM1.value, 5);

    // Set the immediate value
    setBits_num(15, PARAM2.value, 16);

    // CHANGED: Removed the third parameter/register

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void lui_immd_bin(void)
{

    // Check if the opcode bits match "LUI"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "001111") != 0)
    {
        state = WRONG_COMMAND;
        return;
    }

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rt = getBits(20, 5);     // Destination register Rt
    uint32_t imm16 = getBits(15, 16); // Immediate value
                                      // CHANGED: Removed the third parameter/register

    /*
        Set the instruction values
    */

    setOp("LUI");                  // Set the operation to "LUI"
    setParam(1, REGISTER, Rt);     // Set PARAM1 as the destination register
    setParam(2, IMMEDIATE, imm16); // Set PARAM2 as the immediate value
                                   // CHANGED: Removed the third parameter/register

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
