/*
* Author: Ol' Jim
* Editor: Kylie Burke
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "Instruction.h"

void div_reg_assm(void) {

	//Checks if the DIV command if called
	if (strcmp(OP_CODE, "DIV") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// Generally the first parameter should always be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// This is DIV register, so param 2 needs to be a register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Rs should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Rt should be 31 or less
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Create binary

	// Set the 1st half of the OP code
	setBits_str(31, "000000");

	//ALTERED: switched PARAM2 and PARAM1 to match the correct register order

	// Set rs
	setBits_num(25, PARAM1.value, 5);

	// Set rt
	setBits_num(20, PARAM2.value, 5);

	// set 15-6 as 0s
	setBits_num(15, 0, 10);

	// Set the 2nd half of the opcode
	setBits_str(5, "011010");
	
	state = COMPLETE_ENCODE;
}

void div_reg_bin(void) {

	// Check if the op code bits match
	if (checkBits(31, "000000") != 0 || checkBits(5, "011010") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	/*
		Finding values in the binary
	*/
	// getBits(start_bit, width)
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);

	
	/*
		Setting Instruction values
	*/
	setOp("DIV");
	setParam(2, REGISTER, Rs); //first source register operand
	setParam(1, REGISTER, Rt); //second source register operand

	// tell the system the decoding is done
	state = COMPLETE_DECODE;
}


