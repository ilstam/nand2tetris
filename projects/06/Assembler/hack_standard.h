#pragma once

#include <inttypes.h>
#include <stdbool.h>


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

typedef enum sym_id {
    SYM_R0 = 0,
    SYM_R1,
    SYM_R2,
    SYM_R3,
    SYM_R4,
    SYM_R5,
    SYM_R6,
    SYM_R7,
    SYM_R8,
    SYM_R9,
    SYM_R10,
    SYM_R11,
    SYM_R12,
    SYM_R13,
    SYM_R14,
    SYM_R15,
    SYM_SCREEN = 16384,
    SYM_KBD = 24576,
    SYM_SP = 0,
    SYM_LCL = 1,
    SYM_ARG = 2,
    SYM_THIS = 3,
    SYM_THAT = 4,
} sym_id;

typedef struct predef_symbol {
    char name[50+1];
    hack_addr address;
} predef_symbol;

static const predef_symbol predef_symbols[NUM_PREDEFINED_SYMS] = {
    {"R0", SYM_R0}, {"R1", SYM_R1}, {"R2", SYM_R2}, {"R3", SYM_R3}, {"R4", SYM_R4},
    {"R5", SYM_R5}, {"R6", SYM_R6}, {"R7", SYM_R7}, {"R8", SYM_R9}, {"R9", SYM_R9},
    {"R10", SYM_R10}, {"R11", SYM_R11}, {"R12", SYM_R12}, {"R13", SYM_R13},
    {"R14", SYM_R14}, {"R15", SYM_R15}, {"SCREEN", SYM_SCREEN}, {"KBD", SYM_KBD},
    {"SP", SYM_SP}, {"LCL", SYM_LCL}, {"ARG", SYM_ARG}, {"THIS", SYM_THIS},
    {"THAT", SYM_THAT},
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
 * The opcode of an A-instruction is a 0 bit, followed by a 15-bit address.
 * Sometimes we are just provided with a symbol's name instead of a direct
 * address, and we use this symbol to resolve the real address later.
 *
 * When resolved is true that means that we already have a numerical address.
 * When false, the symbol field holds a string that represents a symbol's name.
 */
typedef struct a_inst {
    union {
        char *symbol;
        hack_addr address;
    } operand;
    bool resolved;
} a_inst;

/*
 * The opcode of a C-instruction is the following:
 * 1 1 1 a c1 c2 c3 c4 c5 c6 d1 d2 d3 j1 j2 j3
 */
typedef struct c_inst {
    int16_t prefix:3;
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
