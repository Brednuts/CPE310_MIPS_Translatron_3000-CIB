/*
* Author: Ol' Jim
* Editor: Kylie Burke
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "Instruction.h"

void mfhi_reg_assm(void) {

	// Checking that the op code matches
	// strcmp(string1, string2) return 0 if they match
	if (strcmp(OP_CODE, "MFHI") != 0) {

		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	// The only parameter should be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking the value of parameters
	*/
	// Rd should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
		Putting the binary together
	*/
	// Set 31-16 as 0s
	setBits_num(31, 0, 16);

	// set rd
	setBits_num(15, PARAM1.value, 5);

	// set 10-6 as 0s 
	setBits_str(10, "00000");

	// Set the funct (CHANGED FROM 010010 to 010000)
	setBits_str(5, "010000");

	// tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void mfhi_reg_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	//  any x will be skipped
	// If the manual shows (0), then the value of that bit doesnt matter
	if (checkBits(31, "000000") != 0 || checkBits(5, "010010") != 0 || checkBits(25, "0000000000") != 0 || checkBits(10, "00000") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// If the op code bits match, then the rest can be read as correctly

	/*
		Finding values in the binary
	*/

	// getBits(start_bit, width)
	uint32_t Rd = getBits(15, 5);

	/*
		Setting Instuciton values
	*/
	setOp("MFHI");
	//setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rd); //destination


	// tell the system the decoding is done
	state = COMPLETE_DECODE;
}


