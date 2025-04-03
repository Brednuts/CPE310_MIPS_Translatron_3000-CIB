
/*
 * Author: Ol' Jim
 * Editor: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// NOTE: golden standard code

#include "Instruction.h"

void add_reg_assm(void)
{

    // Check if the opcode matches "ADD"
    if (strcmp(OP_CODE, "ADD") != 0)
    {
        // If the opcode doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Validate the types of parameters
    */

    // The first parameter (PARAM1) must be a register (destination register)
    if (PARAM1.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The second parameter (PARAM2) must be a register (source register Rs)
    if (PARAM2.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    // The third parameter (PARAM3) must be a register (source register Rt)
    if (PARAM3.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    /*
        Validate the values of parameters
    */

    // PARAM1 (Rd) must be a valid register number (0-31)
    if (PARAM1.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM2 (Rs) must be a valid register number (0-31)
    if (PARAM2.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    // PARAM3 (Rt) must be a valid register number (0-31)
    if (PARAM3.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    /*
        Construct the binary instruction
    */

    // Set the opcode to 0 (R-type instruction)
    setBits_num(31, 0, 6);

    // Set Rs (source register)
    setBits_num(25, PARAM2.value, 5);

    // Set Rt (source register)
    setBits_num(20, PARAM3.value, 5);

    // Set Rd (destination register)
    setBits_num(15, PARAM1.value, 5);

    // Set the function code for ADD (100000)
    setBits_str(5, "100000");

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void add_reg_bin(void)
{

    // Check if the opcode and function code match "ADD"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    // Any 'x' in the bit string will be ignored
    if (checkBits(31, "000000") != 0 || checkBits(5, "100000") != 0)
    {
        state = WRONG_COMMAND;
        return;
    }

    // If the opcode and function code match, decode the binary instruction

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rd = getBits(15, 5); // Destination register
    uint32_t Rs = getBits(25, 5); // Source register Rs
    uint32_t Rt = getBits(20, 5); // Source register Rt

    /*
        Set the instruction values
    */

    setOp("ADD");              // Set the operation to "ADD"
    setParam(1, REGISTER, Rd); // Set PARAM1 as the destination register
    setParam(2, REGISTER, Rs); // Set PARAM2 as the first source register
    setParam(3, REGISTER, Rt); // Set PARAM3 as the second source register

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
