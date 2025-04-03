
/*
 * Author: Ol' Jim
 * Editor: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// What was Changed: rs and imm was swapped in the binary instruction

#include "Instruction.h"

void sw_immd_assm(void)
{

	// Check if the opcode matches "SW"
	// strcmp(string1, string2) returns 0 if they match
	if (strcmp(OP_CODE, "SW") != 0)
	{
		// If the opcode doesn't match, this is not the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Validate the types of parameters
	*/

	// The first parameter (PARAM1) must be a register (source register Rt)
	if (PARAM1.type != REGISTER)
	{
		state = MISSING_REG;
		return;
	}

	// The second parameter (PARAM2) must be an immediate value (offset)
	if (PARAM2.type != IMMEDIATE)
	{
		state = INVALID_PARAM;
		return;
	}

	// The third parameter (PARAM3) must be a register (base register Rs)
	if (PARAM3.type != REGISTER)
	{
		state = MISSING_REG;
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

	// PARAM2 (offset) must be a valid 16-bit value (0-0xFFFF)
	if (PARAM2.value > 0xFFFF)
	{
		state = INVALID_IMMED;
		return;
	}

	// PARAM3 (Rs) must be a valid register number (0-31)
	if (PARAM3.value > 31)
	{
		state = INVALID_REG;
		return;
	}

	/*
		Construct the binary instruction
	*/

	// Set the opcode for SW (101011)
	setBits_str(31, "101011");

	// Set Rs (base register)
	setBits_num(25, PARAM3.value, 5);

	// Set Rt (source register)
	setBits_num(20, PARAM1.value, 5);

	// Set the offset (immediate value)
	setBits_num(15, PARAM2.value, 16);

	// Indicate that the encoding is complete
	state = COMPLETE_ENCODE;
}

void sw_immd_bin(void)
{

	// Check if the opcode bits match "SW"
	// checkBits(start_bit, bit_string) returns 0 if the bit_string matches
	if (checkBits(31, "101011") != 0)
	{
		// If the opcode doesn't match, this is not the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Extract values from the binary instruction
	*/

	// getBits(start_bit, width) extracts a value from the binary instruction
	uint32_t Rs = getBits(25, 5);	   // Base register Rs
	uint32_t Rt = getBits(20, 5);	   // Source register Rt
	uint32_t offset = getBits(15, 16); // Immediate value (offset)

	/*
		Set the instruction values
	*/

	setOp("SW"); // Set the operation to "SW"

	// Set PARAM1 as the source register Rt
	setParam(1, REGISTER, Rt);

	// Set PARAM3 as the base register Rs
	setParam(3, REGISTER, Rs); // CHANGED from 2 to 3

	// Set PARAM2 as the immediate value (offset)
	setParam(2, IMMEDIATE, offset); // CHANGED from 3 to 2

	// Indicate that the decoding is complete
	state = COMPLETE_DECODE;
}