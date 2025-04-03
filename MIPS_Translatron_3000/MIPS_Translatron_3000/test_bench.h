#ifndef _TEST_BENCH_H_
#define _TEST_BENCH_H_

/*
    run_tests

    Runs a series of test cases that:
      - Parse an assembly instruction,
      - Encode it into machine code,
      - Decode the machine code back to assembly,
      - And compares the resulting assembly (in canonical form) to the original input.

    Prints the results for each test case and a summary at the end.
*/
void run_tests(void);

#endif