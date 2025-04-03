
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
    
        "AND $t1, $t2, $t3",
        "ADD $s0, $s1, $s2",
        "SUB $t0, $t1, $t2",
        "MULT $a0, $a1, $a2",
        "DIV $t3, $t4, $t5",
        "ADDI $t0, $t1, #0xA",  
        "ANDI $s2, $s3, #0xFF",
        "ORI $t2, $t3, #0x1A",
        "LUI $a0, #0x1000",
        "LW $t0, #0x4, $s1",      // Will be printed as "LW $t0, #0x4($s1)"
        "SW $t0, #0x8, $s1",      // Will be printed as "SW $t0, #0x8($s1)"
        "BEQ $s0, $s1, #0x20",
        "BNE $s2, $s3, #0x30",
        "SLT $t0, $t1, $t2",
        "SLTI $s0, $s1, #0xF" 
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
