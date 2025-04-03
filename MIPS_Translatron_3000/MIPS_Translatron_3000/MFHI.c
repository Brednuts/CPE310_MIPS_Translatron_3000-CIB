
/*
 * Author: Ol' Jim
 * Editor: Kylie Burke and Alyssa Mesinere
 * Date: 03/31/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// What was changed: The function code referenced was incorrect, I changed it from 010010 to 010000

#include "Instruction.h"

void mfhi_reg_assm(void)
{

    // Check if the opcode matches "MFHI"
    // strcmp(string1, string2) returns 0 if they match
    if (strcmp(OP_CODE, "MFHI") != 0)
    {
        // If the opcode doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    /*
        Validate the type of parameters
    */

    // The only parameter (PARAM1) must be a register (destination register Rd)
    if (PARAM1.type != REGISTER)
    {
        state = MISSING_REG;
        return;
    }

    /*
        Validate the value of parameters
    */

    // PARAM1 (Rd) must be a valid register number (0-31)
    if (PARAM1.value > 31)
    {
        state = INVALID_REG;
        return;
    }

    /*
        Construct the binary instruction
    */

    // Set bits 31-16 to 0 (unused for MFHI)
    setBits_num(31, 0, 16);

    // Set Rd (destination register)
    setBits_num(15, PARAM1.value, 5);

    // Set bits 10-6 to 0 (unused for MFHI)
    setBits_str(10, "00000");

    // Set the function code for MFHI (010000)
    // CHANGED: Updated from 010010 to 010000
    setBits_str(5, "010000");

    // Indicate that the encoding is complete
    state = COMPLETE_ENCODE;
}

void mfhi_reg_bin(void)
{

    // Check if the opcode and function code match "MFHI"
    // checkBits(start_bit, bit_string) returns 0 if the bit_string matches
    // Any 'x' in the bit string will be ignored
    if (checkBits(31, "000000") != 0 || checkBits(5, "010000") != 0 || checkBits(25, "0000000000") != 0 || checkBits(10, "00000") != 0)
    { // CHANGED: Updated function code check from 010010 to 010000
        // If the opcode or function code doesn't match, this is not the correct command
        state = WRONG_COMMAND;
        return;
    }

    // If the opcode and function code match, decode the binary instruction

    /*
        Extract values from the binary instruction
    */

    // getBits(start_bit, width) extracts a value from the binary instruction
    uint32_t Rd = getBits(15, 5); // Destination register Rd

    /*
        Set the instruction values
    */

    setOp("MFHI");             // Set the operation to "MFHI"
    setParam(1, REGISTER, Rd); // Set PARAM1 as the destination register Rd

    // Indicate that the decoding is complete
    state = COMPLETE_DECODE;
}
