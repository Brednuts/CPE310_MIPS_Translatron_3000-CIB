
/*
 * Author: Kylie Burke
 * Date: 04/02/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

#include "Instruction.h"

void slti_immd_assm(void)
{

	// Check if the opcode matches "SLTI"
	// strcmp(string1, string2) returns 0 if they match
	if (strcmp(OP_CODE, "SLTI") != 0)
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

	// The second parameter (PARAM2) must be a register (source register Rs)
	if (PARAM2.type != REGISTER)
	{
		state = MISSING_REG;
		return;
	}

	// The third parameter (PARAM3) must be an immediate value
	if (PARAM3.type != IMMEDIATE)
	{
		state = INVALID_PARAM;
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

	// PARAM2 (Rs) must be a valid register number (0-31)
	if (PARAM2.value > 31)
	{
		state = INVALID_REG;
		return;
	}

	// PARAM3 (immediate) must be a valid 16-bit value (0-0xFFFF)
	if (PARAM3.value > 0xFFFF)
	{
		state = INVALID_IMMED;
		return;
	}

	/*
		Construct the binary instruction
	*/

	// Set the opcode for SLTI (001010)
	setBits_str(31, "001010");

	// Set Rs (source register)
	setBits_num(25, PARAM2.value, 5);

	// Set Rt (destination register)
	setBits_num(20, PARAM1.value, 5);

	// Set the immediate value
	setBits_num(15, PARAM3.value, 16);

	// Indicate that the encoding is complete
	state = COMPLETE_ENCODE;
}

void slti_immd_bin(void)
{

	// Check if the opcode bits match "SLTI"
	// checkBits(start_bit, bit_string) returns 0 if the bit_string matches
	if (checkBits(31, "001010") != 0)
	{
		// If the opcode doesn't match, this is not the correct command
		state = WRONG_COMMAND;
		return;
	}

	// If the opcode bits match, decode the binary instruction

	/*
		Extract values from the binary instruction
	*/

	// getBits(start_bit, width) extracts a value from the binary instruction
	uint32_t Rs = getBits(25, 5);	   // Source register Rs
	uint32_t Rt = getBits(20, 5);	   // Destination register Rt
	uint32_t offset = getBits(15, 16); // Immediate value

	/*
		Set the instruction values
	*/

	setOp("SLTI"); // Set the operation to "SLTI"

	// Set PARAM1 as the destination register Rt
	setParam(1, REGISTER, Rt);

	// Set PARAM2 as the source register Rs
	setParam(2, REGISTER, Rs);

	// Set PARAM3 as the immediate value
	setParam(3, IMMEDIATE, offset);

	// Indicate that the decoding is complete
	state = COMPLETE_DECODE;
}
