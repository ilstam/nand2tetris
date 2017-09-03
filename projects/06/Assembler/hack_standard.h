#pragma once

#include <inttypes.h>


#define MAX_HACK_ADDRESS INT16_MAX

/*
 * We cannot support more than MAX_HACK_ADDRESS instructions because we are
 * not able to address them.
 */
#define MAX_INSTRUCTION MAX_HACK_ADDRESS

/*
 * Hack addresses are 15-bit unsigned values. By using a 16-bit signed value
 * we can also represent invalid addresses (all negative values).
 */
typedef int16_t hack_addr;


/*
 * Total number of predefined symbols.
 */
#define NUM_PREDEFINED_SYMS 23

typedef struct predef_symbol {
    char name[50+1];
    hack_addr address;
} predef_symbol;

static const predef_symbol predef_symbols[NUM_PREDEFINED_SYMS] = {
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5},
    {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11},
    {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}, {"SCREEN", 16384},
    {"KBD", 24576}, {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3},
    {"THAT", 4},
};

/*
 * Represents A-instructions, C-instructions and invalid instructions.
 */
typedef enum inst_id {
    INST_INVALID = -1,
    INST_A,
    INST_C,
} inst_id;

/*
 * Stores a string that represents either a symbol or a numerical address.
 * We do NOT store the @ symbol of the instruction.
 * e.g. possible values: "LOOP" or "13241"
 *
 * The opcode of an A-instruction is a 0 bit, followed by a 15-bit address.
 * Sometimes we are just provided with a symbol's name instead of a direct
 * address, and we use this symbol to resolve the real address later.
 */
typedef char *a_inst;

/*
 * The opcode of a C-instruction is the following:
 * 1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
 */
typedef struct c_inst {
    int16_t a:1;
    int16_t comp:6;
    int16_t dest:3;
    int16_t jump:3;
} c_inst ;

/*
 * A generic instruction that can hold either an A-instruction or a C-instruction.
 * In order to determine which type of instruction it holds we have to check
 * the id field.
 */
typedef struct generic_inst {
    union {
        c_inst c;
        a_inst a;
    } inst;
    inst_id id;
} generic_inst;
