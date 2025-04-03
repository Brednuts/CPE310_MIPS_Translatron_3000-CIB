
/*
 * Author: Ol' Jim
 * Editor: Brennan Walsh
 * Date: 03/30/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

// What was Changed: The function code referenced was incorrect, I changed it from 100100 to 100010
#include "Instruction.h"

void sub_reg_assm(void)
{

	// Check if the opcode matches "SUB"
	// strcmp(string1, string2) returns 0 if they match
	if (strcmp(OP_CODE, "SUB") != 0)
	{
		// If the opcode doesn't match, this is not the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Validate the types of parameters
	*/

	// The first parameter (PARAM1) must be a register (destination register Rd)
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

	// Set the opcode for SUB (R-type instruction, opcode = 0)
	setBits_num(31, 0, 6);

	// Set the function code for SUB (100010)
	setBits_str(5, "100010");

	// Set Rd (destination register)
	setBits_num(15, PARAM1.value, 5);

	// Set Rs (source register)
	setBits_num(25, PARAM2.value, 5);

	// Set Rt (source register)
	setBits_num(20, PARAM3.value, 5);

	// Indicate that the encoding is complete
	state = COMPLETE_ENCODE;
}

void sub_reg_bin(void)
{

	// Check if the opcode and function code match "SUB"
	// checkBits(start_bit, bit_string) returns 0 if the bit_string matches
	// Any 'x' in the bit string will be ignored
	// If the manual shows (0), then the value of that bit doesn't matter

	// CHANGED: Corrected the second checkBits to compare "100010" instead of "100100"
	if (checkBits(31, "000000") != 0 || checkBits(5, "100010") != 0)
	{
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
	uint32_t Rs = getBits(25, 5); // Source register Rs
	uint32_t Rt = getBits(20, 5); // Source register Rt

	/*
		Set the instruction values
	*/

	setOp("SUB"); // Set the operation to "SUB"

	// Set PARAM1 as the destination register Rd
	setParam(1, REGISTER, Rd);

	// Set PARAM2 as the first source register Rs
	setParam(2, REGISTER, Rs);

	// Set PARAM3 as the second source register Rt
	setParam(3, REGISTER, Rt);

	// Indicate that the decoding is complete
	state = COMPLETE_DECODE;
}
