
/*
 * Author: Ol' Jim
 * Editors: Kylie Burke and Alyssa Mesinere
 * Date: 03/31/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// What was Changed: The rt and rs registers were flipped in the binary and machine code

#include "Instruction.h"

void div_reg_assm(void)
{

    // Check if the opcode matches "DIV"
    if (strcmp(OP_CODE, "DIV") != 0)
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

    /*
        Construct the binary instruction
    */

    // Set the opcode for DIV (R-type instruction, opcode = 0)
    setBits_str(31, "000000");

    // Set Rs (source register)
    setBits_num(25, PARAM1.value, 5); // CHANGED PARAM2 and PARAM1

    // Set Rt (source register)
    setBits_num(20, PARAM2.value, 5); // CHANGED PARAM1 and PARAM2

    // Set bits 15-6 to 0 (unused for DIV)
    setBits_num(15, 0, 10);

    // Set the function code for DIV (011010)
    setBits_str(5, "011010");

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void div_reg_bin(void)
{

    // Check if the opcode and function code match "DIV"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "000000") != 0 || checkBits(5, "011010") != 0)
    {
        state = WRONG_COMMAND;
        return;
    }

    // If the opcode and function code match, decode the binary instruction

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rs = getBits(25, 5); // Source register Rs
    uint32_t Rt = getBits(20, 5); // Source register Rt

    /*
        Set the instruction values
    */

    setOp("DIV"); // Set the operation to "DIV"

    // Set PARAM1 as the first source register Rs
    setParam(1, REGISTER, Rs); // CHANGED 1 to 2

    // Set PARAM2 as the second source register Rt
    setParam(2, REGISTER, Rt); // CHANGED 2 to 1

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
