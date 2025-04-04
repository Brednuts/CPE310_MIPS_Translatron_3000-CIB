
/*
 * Authors: Aidan Lett and Brendan Walsh
 * Date: 04/03/2025
 * ByteForge Systems
 * MIPS-Translatron 3000
 */

#include "test_bench.h"
#include "MIPS_Interpreter.h"  // To access initAll, parseAssem, encode, decode, etc.
#include "global_data.h"       // For the global assm_instruct and state.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ASM_BUFFER_SIZE 200

/*
    reg_to_str

    Converts a register number into its corresponding string representation.
    Uses the same register mapping as printParam function.
    The result is written into 'temp' (of size temp_size) if needed.
    Returns a pointer to a string representing the register.
*/
static const char* reg_to_str(uint32_t reg, char* temp, size_t temp_size)
{
    if (reg == 0) 
    {
        return "$zero";
    } 
    else if (reg == 2 || reg == 3) 
    {
        snprintf(temp, temp_size, "$v%d", reg - 2);
        return temp;
    } 
    else if (reg >= 4 && reg <= 7) 
    {
        snprintf(temp, temp_size, "$a%d", reg - 4);
        return temp;
    } 
    else if (reg >= 8 && reg <= 15) 
    {
        snprintf(temp, temp_size, "$t%d", reg - 8);
        return temp;
    } 
    else if (reg >= 16 && reg <= 23) 
    {
        snprintf(temp, temp_size, "$s%d", reg - 16);
        return temp;
    }
    else if (reg == 24 || reg == 25) 
    {
        snprintf(temp, temp_size, "$t%d", reg - 16);
        return temp;
    } 
    else if (reg == 28) 
    {
        return "$gp";
    } 
    else if (reg == 29) 
    {
        return "$sp";
    } 
    else if (reg == 30) 
    {
        return "$fp";
    }
    else if (reg == 31) 
    {
        return "$ra";
    } 
    else 
    {
        return "<unknown>";
    }
}

/*
    getAssemblyString

    Fills the provided buffer with the assembly instruction generated from the global 
    'assm_instruct' structure. This function mimics the behavior of printAssm(), but 
    writes into a string for automated comparison.
*/
static void getAssemblyString(char *buf, size_t buf_size) 
{
    buf[0] = '\0';

    // Append opcode.
    strncat(buf, assm_instruct.op, buf_size - strlen(buf) - 1);
    strncat(buf, " ", buf_size - strlen(buf) - 1);

    char temp[50];

    // For param1.
    if (assm_instruct.param1.type != EMPTY) 
    {
        if (assm_instruct.param1.type == REGISTER) 
        {
            strncat(buf, reg_to_str(assm_instruct.param1.value, temp, sizeof(temp)), 
                    buf_size - strlen(buf) - 1);
        }
        else if (assm_instruct.param1.type == IMMEDIATE)
        {
            snprintf(temp, sizeof(temp), "#0x%X", assm_instruct.param1.value);
            strncat(buf, temp, buf_size - strlen(buf) - 1);
        }
        else 
        {
            strncat(buf, "<>", buf_size - strlen(buf) - 1);
        }
    }

    // For param2.
    if (assm_instruct.param2.type != EMPTY) 
    {
        strncat(buf, ", ", buf_size - strlen(buf) - 1);
        if (assm_instruct.param2.type == REGISTER) 
        {
            strncat(buf, reg_to_str(assm_instruct.param2.value, temp, sizeof(temp)), 
                    buf_size - strlen(buf) - 1);
        }
        else if (assm_instruct.param2.type == IMMEDIATE) 
        {
            snprintf(temp, sizeof(temp), "#0x%X", assm_instruct.param2.value);
            strncat(buf, temp, buf_size - strlen(buf) - 1);
        }
        else 
        {
            strncat(buf, "<>", buf_size - strlen(buf) - 1);
        }
    }

   // For param3.
   if (assm_instruct.param3.type != EMPTY) 
   {
    /* For LW and SW, print a comma followed by the register */
    if (assm_instruct.param3.type == REGISTER &&
        (strcmp(assm_instruct.op, "LW") == 0 || strcmp(assm_instruct.op, "SW") == 0)) 
    {
        strncat(buf, ", ", buf_size - strlen(buf) - 1);
        strncat(buf, reg_to_str(assm_instruct.param3.value, temp, sizeof(temp)),
                buf_size - strlen(buf) - 1);
    }
    else 
    {
        strncat(buf, ", ", buf_size - strlen(buf) - 1);
        if (assm_instruct.param3.type == REGISTER) 
        {
            strncat(buf, reg_to_str(assm_instruct.param3.value, temp, sizeof(temp)),
                    buf_size - strlen(buf) - 1);
        }
        else if (assm_instruct.param3.type == IMMEDIATE) 
        {
            snprintf(temp, sizeof(temp), "#0x%X", assm_instruct.param3.value);
            strncat(buf, temp, buf_size - strlen(buf) - 1);
        }
        else 
        {
            strncat(buf, "<>", buf_size - strlen(buf) - 1);
        }
    }
}

    // For param4.
    if (assm_instruct.param4.type != EMPTY) 
    {
        strncat(buf, ", ", buf_size - strlen(buf) - 1);
        if (assm_instruct.param4.type == REGISTER) 
        {
            strncat(buf, reg_to_str(assm_instruct.param4.value, temp, sizeof(temp)), 
                    buf_size - strlen(buf) - 1);
        }
        else if (assm_instruct.param4.type == IMMEDIATE) 
        {
            snprintf(temp, sizeof(temp), "#0x%X", assm_instruct.param4.value);
            strncat(buf, temp, buf_size - strlen(buf) - 1);
        }
        else 
        {
            strncat(buf, "<>", buf_size - strlen(buf) - 1);
        }
    }

    strncat(buf, "\n", buf_size - strlen(buf) - 1);
}

/*
    trim

    Removes any trailing spaces and newline characters from the string.
*/
static void trim(char *str) 
{
    size_t len = strlen(str);
    while(len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\r')) 
    {
        str[len - 1] = '\0';
        len--;
    }
}

/*
    run_test_case

    Performs a single test:
      - Initializes the global state,
      - Parses the given assembly string,
      - Encodes it,
      - Decodes it back to assembly,
      - Retrieves the disassembled assembly string,
      - And compares it to the original input (after trimming).

    Returns 1 if the test passes, 0 otherwise, and prints details to stdout.
*/
static int run_test_case(const char *test_input) 
{
    char result[ASM_BUFFER_SIZE];
    char input_copy[ASM_BUFFER_SIZE];
    strncpy(input_copy, test_input, ASM_BUFFER_SIZE - 1);
    input_copy[ASM_BUFFER_SIZE - 1] = '\0';

    initAll(); // Resets global state.

    parseAssem(input_copy);
    if (state != NO_ERROR) 
    {
        printf("Test FAILED for input: \"%s\"\n", test_input);
        printf("  Error during parsing: state=%d\n", state);
        return 0;
    }

    encode();
    if (state != NO_ERROR && state != COMPLETE_ENCODE) 
    {
        printf("Test FAILED for input: \"%s\"\n", test_input);
        printf("  Error during encoding: state=%d\n", state);
        return 0;
    }

    decode();
    if (state != NO_ERROR && state != COMPLETE_DECODE) 
    {
        printf("Test FAILED for input: \"%s\"\n", test_input);
        printf("  Error during decoding: state=%d\n", state);
        return 0;
    }

    getAssemblyString(result, ASM_BUFFER_SIZE);
    trim(result);

    char expected[ASM_BUFFER_SIZE];
    strncpy(expected, test_input, ASM_BUFFER_SIZE - 1);
    expected[ASM_BUFFER_SIZE - 1] = '\0';
    trim(expected);

    if (strcmp(expected, result) == 0) 
    {
        printf("Test PASSED for input: \"%s\"\n", test_input);
        return 1;
    }
    else 
    {
        printf("Test FAILED for input: \"%s\"\n", test_input);
        printf("  Expected: \"%s\"\n", expected);
        printf("  Got:      \"%s\"\n", result);
        return 0;
    }
}

/*
    run_tests

    Runs an array of test cases and reports a summary of pass/fail counts.
*/
void run_tests(void) 
{
    const char *test_cases[] = {
        // ADD test cases
        "ADD $t0, $t1, $t2",
        "ADD $t3, $t4, $t5",
        "ADD $s0, $s1, $s2",
        "ADD $s3, $s4, $s5",
        "ADD $a0, $a1, $a2",
        "ADD $t6, $t7, $s0",
        "ADD $s6, $s7, $t0",
        "ADD $t1, $t2, $t3",
        "ADD $s2, $s3, $s4",
        "ADD $a2, $a3, $s5",
    
        // ADDI test cases
        "ADDI $t0, $t1, #0x1",
        "ADDI $s0, $s1, #0x2",
        "ADDI $t2, $t3, #0x3",
        "ADDI $s2, $s3, #0x4",
        "ADDI $a0, $a1, #0x5",
        "ADDI $t4, $t5, #0x6",
        "ADDI $s4, $s5, #0x7",
        "ADDI $t6, $t7, #0x8",
        "ADDI $s6, $s7, #0x9",
        "ADDI $a2, $a3, #0xA",
    
        // AND test cases
        "AND $t0, $t1, $t2",
        "AND $s0, $s1, $s2",
        "AND $t3, $t4, $t5",
        "AND $s3, $s4, $s5",
        "AND $a0, $a1, $a2",
        "AND $t6, $t7, $s0",
        "AND $s6, $s7, $t0",
        "AND $t1, $t2, $t3",
        "AND $s2, $s3, $s4",
        "AND $a2, $a3, $s5",
    
        // ANDI test cases
        "ANDI $t0, $t1, #0xFF",
        "ANDI $s0, $s1, #0x1F",
        "ANDI $t2, $t3, #0x2A",
        "ANDI $s2, $s3, #0x3C",
        "ANDI $a0, $a1, #0x4D",
        "ANDI $t4, $t5, #0x5E",
        "ANDI $s4, $s5, #0x6F",
        "ANDI $t6, $t7, #0x7A",
        "ANDI $s6, $s7, #0x8B",
        "ANDI $a2, $a3, #0x9C",
    
        // BEQ test cases
        "BEQ $t0, $t1, #0x10",
        "BEQ $s0, $s1, #0x20",
        "BEQ $t2, $t3, #0x30",
        "BEQ $s2, $s3, #0x40",
        "BEQ $a0, $a1, #0x50",
        "BEQ $t4, $t5, #0x60",
        "BEQ $s4, $s5, #0x70",
        "BEQ $t6, $t7, #0x80",
        "BEQ $s6, $s7, #0x90",
        "BEQ $a2, $a3, #0xA0",
    
        // BNE test cases
        "BNE $t0, $t1, #0x11",
        "BNE $s0, $s1, #0x21",
        "BNE $t2, $t3, #0x31",
        "BNE $s2, $s3, #0x41",
        "BNE $a0, $a1, #0x51",
        "BNE $t4, $t5, #0x61",
        "BNE $s4, $s5, #0x71",
        "BNE $t6, $t7, #0x81",
        "BNE $s6, $s7, #0x91",
        "BNE $a2, $a3, #0xA1",
    
        // DIV test cases
        "DIV $t0, $t1, $t2",
        "DIV $s0, $s1, $s2",
        "DIV $t3, $t4, $t5",
        "DIV $s3, $s4, $s5",
        "DIV $a0, $a1, $a2",
        "DIV $t6, $t7, $s0",
        "DIV $s6, $s7, $t0",
        "DIV $t1, $t2, $t3",
        "DIV $s2, $s3, $s4",
        "DIV $a2, $a3, $s5",
    
        // LUI test cases
        "LUI $t0, #0x1000",
        "LUI $s0, #0x2000",
        "LUI $t1, #0x3000",
        "LUI $s1, #0x4000",
        "LUI $a0, #0x5000",
        "LUI $t2, #0x6000",
        "LUI $s2, #0x7000",
        "LUI $t3, #0x8000",
        "LUI $s3, #0x9000",
        "LUI $a1, #0xA000",
    
        // LW test cases
        "LW $t0, #0x4, $s1",
        "LW $s0, #0x8, $t0",
        "LW $t1, #0xC, $s1",
        "LW $s1, #0x10, $t1",
        "LW $a0, #0x14, $s2",
        "LW $t2, #0x18, $s3",
        "LW $s2, #0x1C, $t3",
        "LW $t3, #0x20, $s4",
        "LW $s3, #0x24, $t4",
        "LW $a1, #0x28, $s5",
    
        // MFHI test cases
        "MFHI $t0",
        "MFHI $s0",
        "MFHI $t1",
        "MFHI $s1",
        "MFHI $a0",
        "MFHI $t2",
        "MFHI $s2",
        "MFHI $t3",
        "MFHI $s3",
        "MFHI $a1",
    
        // MFLO test cases
        "MFLO $t0",
        "MFLO $s0",
        "MFLO $t1",
        "MFLO $s1",
        "MFLO $a0",
        "MFLO $t2",
        "MFLO $s2",
        "MFLO $t3",
        "MFLO $s3",
        "MFLO $a1",
    
        // MULT test cases
        "MULT $t0, $t1, $t2",
        "MULT $s0, $s1, $s2",
        "MULT $t3, $t4, $t5",
        "MULT $s3, $s4, $s5",
        "MULT $a0, $a1, $a2",
        "MULT $t6, $t7, $s0",
        "MULT $s6, $s7, $t0",
        "MULT $t1, $t2, $t3",
        "MULT $s2, $s3, $s4",
        "MULT $a2, $a3, $s5",
    
        // OR test cases
        "OR $t0, $t1, $t2",
        "OR $s0, $s1, $s2",
        "OR $t3, $t4, $t5",
        "OR $s3, $s4, $s5",
        "OR $a0, $a1, $a2",
        "OR $t6, $t7, $s0",
        "OR $s6, $s7, $t0",
        "OR $t1, $t2, $t3",
        "OR $s2, $s3, $s4",
        "OR $a2, $a3, $s5",
    
        // ORI test cases
        "ORI $t0, $t1, #0x1A",
        "ORI $s0, $s1, #0x2B",
        "ORI $t2, $t3, #0x3C",
        "ORI $s2, $s3, #0x4D",
        "ORI $a0, $a1, #0x5E",
        "ORI $t4, $t5, #0x6F",
        "ORI $s4, $s5, #0x7A",
        "ORI $t6, $t7, #0x8B",
        "ORI $s6, $s7, #0x9C",
        "ORI $a2, $a3, #0xAD",
    
        // SLT test cases
        "SLT $t0, $t1, $t2",
        "SLT $s0, $s1, $s2",
        "SLT $t3, $t4, $t5",
        "SLT $s3, $s4, $s5",
        "SLT $a0, $a1, $a2",
        "SLT $t6, $t7, $s0",
        "SLT $s6, $s7, $t0",
        "SLT $t1, $t2, $t3",
        "SLT $s2, $s3, $s4",
        "SLT $a2, $a3, $s5",
    
        // SLTI test cases
        "SLTI $t0, $t1, #0xA",
        "SLTI $s0, $s1, #0xB",
        "SLTI $t2, $t3, #0xC",
        "SLTI $s2, $s3, #0xD",
        "SLTI $a0, $a1, #0xE",
        "SLTI $t4, $t5, #0xF",
        "SLTI $s4, $s5, #0x10",
        "SLTI $t6, $t7, #0x11",
        "SLTI $s6, $s7, #0x12",
        "SLTI $a2, $a3, #0x13",
    
        // SUB test cases
        "SUB $t0, $t1, $t2",
        "SUB $s0, $s1, $s2",
        "SUB $t3, $t4, $t5",
        "SUB $s3, $s4, $s5",
        "SUB $a0, $a1, $a2",
        "SUB $t6, $t7, $s0",
        "SUB $s6, $s7, $t0",
        "SUB $t1, $t2, $t3",
        "SUB $s2, $s3, $s4",
        "SUB $a2, $a3, $s5",
    
        // SW test cases
        "SW $t0, #0x4, $s1",
        "SW $s0, #0x8, $t0",
        "SW $t1, #0xC, $s1",
        "SW $s1, #0x10, $t1",
        "SW $a0, #0x14, $s2",
        "SW $t2, #0x18, $s3",
        "SW $s2, #0x1C, $t3",
        "SW $t3, #0x20, $s4",
        "SW $s3, #0x24, $t4",
        "SW $a1, #0x28, $s5"
    };
    const int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    int passed = 0;

    printf("Running %d test case(s)...\n\n", num_tests);
    for (int i = 0; i < num_tests; i++) 
    {
        if (run_test_case(test_cases[i]))
            passed++;
    }
    printf("\nTest bench results: %d/%d test(s) passed.\n", passed, num_tests);
}
