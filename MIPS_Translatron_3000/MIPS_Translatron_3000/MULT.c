
/*
 * Author: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

#include "Instruction.h"

void mult_reg_assm(void)
{

    // Check if the opcode matches "MULT"
    // strcmp(string1, string2) returns 0 if they match
    if (strcmp(OP_CODE, "MULT") != 0)
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

    // Set the opcode for MULT (R-type instruction, opcode = 0)
    setBits_str(31, "000000");

    // Set Rs (source register)
    // ALTERED: Switched PARAM2 and PARAM1 to match the correct register order
    setBits_num(25, PARAM1.value, 5);

    // Set Rt (source register)
    setBits_num(20, PARAM2.value, 5);

    // Set bits 15-6 to 0 (unused for MULT)
    setBits_num(15, 0, 10);

    // Set the function code for MULT (011000)
    setBits_str(5, "011000");

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void mult_reg_bin(void)
{

    // Check if the opcode and function code match "MULT"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    if (checkBits(31, "000000") != 0 || checkBits(5, "011000") != 0)
    {
        // If the opcode or function code doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rs = getBits(25, 5); // Source register Rs
    uint32_t Rt = getBits(20, 5); // Source register Rt

    /*
        Set the instruction values
    */

    setOp("MULT"); // Set the operation to "MULT"

    // Set PARAM1 as the first source register Rs
    setParam(1, REGISTER, Rs);

    // Set PARAM2 as the second source register Rt
    setParam(2, REGISTER, Rt);

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
