/**
 * Author: Brendan Walsh
 * Date: 03/30/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

#include "Instruction.h"

void or_reg_assm(void) {
	// Checking that the op code matches
	// strcmp(string1, string2) returns 0 if they match
	if (strcmp(OP_CODE, "OR") != 0) {
		// If op code doesn't match, this isn't the correct command 
		state = WRONG_COMMAND;
		return;
	}

	/*
	Checking the type of parameters 
	*/

	// First parameter should be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Second parameter should be a register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}
	// Third parameter should be a register 
	if (PARAM3.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking value of parameters 
	*/

	// Destination Register (Rd) should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}
	// Source register 1 (Rs) should be 31 or less
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}
	// Source register 2 (Rt) should be 31 or less 
	if (PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Set the opcode 
	setBits_num(31, 0, 6);

	// Set the function 
	setBits_str(5, "100101");

	// set Destination register (Rd)
	setBits_num(15, PARAM1.value, 5);

	// set Source register 1 (Rs)
	setBits_num(25, PARAM2.value, 5);

	// set Source register 2 (Rt)
	setBits_num(20, PARAM3.value, 5);

	// tell the system encoding is done 
	state = COMPLETE_ENCODE;
}

void or_reg_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches 
	// 	any x will be skipped
	// If the manual shows (0), then the value of that bit doesn't matter 
	if (checkBits(31, "000000") != 0 || checkBits(5, "100101") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// If the op code bits match, then the rest can be read correctly 

	/*
		Finding values in binary 
	*/
	// getBits(start_bit, width); 
	uint32_t Rd = getBits(15, 5);
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);

	/*
		Setting Instruction values 
	*/

	// Set op code to "OR"
	setOp("OR");

	setParam(1, REGISTER, Rd); // destination register
	setParam(2, REGISTER, Rs); // first source register operand
	setParam(3, REGISTER, Rt); // second source register operand 

	// tell the system that decoding is done 
	state = COMPLETE_DECODE;
}


