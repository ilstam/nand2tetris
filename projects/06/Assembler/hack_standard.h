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


typedef enum jump_id {
    JMP_INVALID= -1,
    JMP_NULL,
    JMP_JGT,
    JMP_JEQ,
    JMP_JGE,
    JMP_JLT,
    JMP_JNE,
    JMP_JLE,
    JMP_JMP,
} jump_id;

typedef enum dest_id {
    DEST_INVALID = -1,
    DEST_NULL,
    DEST_M,
    DEST_D,
    DEST_MD,
    DEST_A,
    DEST_AM,
    DEST_AD,
    DEST_AMD,
} dest_id;

typedef enum comp_id {
    COMP_INVALID = -1,

    /* for a = 0 */

    COMP_0 = 42,
    COMP_1 = 63,
    COMP_MINUS_1 = 58,
    COMP_D = 12,
    COMP_A = 48,
    COMP_NOT_D = 13,
    COMP_NOT_A = 49,
    COMP_MINUS_D = 15,
    COMP_MINUS_A = 51,
    COMP_D_PLUS_1 = 31,
    COMP_A_PLUS_1 = 55,
    COMP_D_MINUS_1 = 14,
    COMP_A_MINUS_1 = 50,
    COMP_D_PLUS_A = 2,
    COMP_D_MINUS_A = 19,
    COMP_A_MINUS_D = 7,
    COMP_D_AND_A = 0,
    COMP_D_OR_A = 21,

    /* for a = 1 */

    COMP_M = 48,
    COMP_NOT_M = 49,
    COMP_MINUS_M = 51,
    COMP_M_PLUS_1 = 55,
    COMP_M_MINUS_1 = 50,
    COMP_D_PLUS_M = 2,
    COMP_D_MINUS_M = 19,
    COMP_M_MINUS_D = 7,
    COMP_D_AND_M = 0,
    COMP_D_OR_M = 21,
} comp_id;

static inline dest_id str_to_destid(const char *s)
{
    dest_id id = DEST_INVALID;

    if (s == NULL) {
        id = DEST_NULL;
    } else if (!strcmp(s, "M")) {
        id = DEST_M;
    } else if (!strcmp(s, "D")) {
        id = DEST_D;
    } else if (!strcmp(s, "MD")) {
        id = DEST_MD;
    } else if (!strcmp(s, "A")) {
        id = DEST_A;
    } else if (!strcmp(s, "AM")) {
        id = DEST_AM;
    } else if (!strcmp(s, "AD")) {
        id = DEST_AD;
    } else if (!strcmp(s, "AMD")) {
        id = DEST_AMD;
    }

    return id;
}

static inline comp_id str_to_compid(const char *s, int *a)
{
    comp_id id = COMP_INVALID;

    if (s == NULL) {
        return id;
    } else if (!strcmp(s, "0")) {
        id = COMP_0;
    } else if (!strcmp(s, "1")) {
        id = COMP_1;
    } else if (!strcmp(s, "-1")) {
        id = COMP_MINUS_1;
    } else if (!strcmp(s, "D")) {
        id = COMP_D;
    } else if (!strcmp(s, "A")) {
        id = COMP_A;
    } else if (!strcmp(s, "!D")) {
        id = COMP_NOT_D;
    } else if (!strcmp(s, "!A")) {
        id = COMP_NOT_A;
    } else if (!strcmp(s, "-D")) {
        id = COMP_MINUS_D;
    } else if (!strcmp(s, "-A")) {
        id = COMP_MINUS_A;
    } else if (!strcmp(s, "D+1")) {
        id = COMP_D_PLUS_1;
    } else if (!strcmp(s, "A+1")) {
        id = COMP_A_PLUS_1;
    } else if (!strcmp(s, "D-1")) {
        id = COMP_D_MINUS_1;
    } else if (!strcmp(s, "A-1")) {
        id = COMP_A_MINUS_1;
    } else if (!strcmp(s, "D+A")) {
        id = COMP_D_PLUS_A;
    } else if (!strcmp(s, "D-A")) {
        id = COMP_D_MINUS_A;
    } else if (!strcmp(s, "A-D")) {
        id = COMP_A_MINUS_D;
    } else if (!strcmp(s, "D&A")) {
        id = COMP_D_AND_A;
    } else if (!strcmp(s, "D|A")) {
        id = COMP_D_OR_A;
    }

    if (id != COMP_INVALID) {
        *a = 0;
        return id;
    }

    if (!strcmp(s, "M")) {
        id = COMP_M;
    } else if (!strcmp(s, "!M")) {
        id = COMP_NOT_M;
    } else if (!strcmp(s, "-M")) {
        id = COMP_MINUS_M;
    } else if (!strcmp(s, "M+1")) {
        id = COMP_M_PLUS_1;
    } else if (!strcmp(s, "M-1")) {
        id = COMP_M_MINUS_1;
    } else if (!strcmp(s, "D+M")) {
        id = COMP_D_PLUS_M;
    } else if (!strcmp(s, "D-M")) {
        id = COMP_D_MINUS_M;
    } else if (!strcmp(s, "M-D")) {
        id = COMP_M_MINUS_D;
    } else if (!strcmp(s, "D&M")) {
        id = COMP_D_AND_M;
    } else if (!strcmp(s, "D|M")) {
        id = COMP_D_OR_M;
    }

    *a = 1;
    return id;
}

static inline jump_id str_to_jumpid(const char *s)
{
    jump_id id = JMP_INVALID;

    if (s == NULL) {
        id = JMP_NULL;
    } else if (!strcmp(s, "JGT")) {
        id = JMP_JGT;
    } else if (!strcmp(s, "JEQ")) {
        id = JMP_JEQ;
    } else if (!strcmp(s, "JGE")) {
        id = JMP_JGE;
    } else if (!strcmp(s, "JLT")) {
        id = JMP_JLT;
    } else if (!strcmp(s, "JNE")) {
        id = JMP_JNE;
    } else if (!strcmp(s, "JLE")) {
        id = JMP_JLE;
    } else if (!strcmp(s, "JMP")) {
        id = JMP_JMP;
    }

    return id;
}


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
    int16_t a:1;
    int16_t comp:7; // make these bigger than their opcode length in order to be able
    int16_t dest:4; // be able to hold invalid values as well
    int16_t jump:4;
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
