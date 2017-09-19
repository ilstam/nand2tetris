#pragma once


#define ASM_PUSH_CONST \
    "@%d\n"            \
    "D=A\n"            \
    "@SP\n"            \
    "AM=M+1\n"         \
    "A=A-1\n"          \
    "M=D\n"

#define ASM_PUSH_STATIC \
    "@%s.%d\n"           \
    "D=M\n"             \
    "@SP\n"             \
    "AM=M+1\n"          \
    "A=A-1\n"           \
    "M=D\n"

// local, argument, this, that
#define ASM_PUSH_LATT \
    "@%d\n"           \
    "D=A\n"           \
    "@%s\n"           \
    "A=M\n"           \
    "A=D+A\n"         \
    "D=M\n"           \
    "@SP\n"           \
    "AM=M+1\n"        \
    "A=A-1\n"         \
    "M=D\n"

#define ASM_PUSH_TEMP \
    "@5\n"            \
    "D=A\n"           \
    "@%d\n"           \
    "A=D+A\n"         \
    "D=M\n"           \
    "@SP\n"           \
    "AM=M+1\n"        \
    "A=A-1\n"         \
    "M=D\n"

#define ASM_PUSH_POINTER \
    "@%s\n"              \
    "D=M\n"              \
    "@SP\n"              \
    "AM=M+1\n"           \
    "A=A-1\n"            \
    "M=D\n"

#define ASM_POP_STATIC \
    "@SP\n"            \
    "AM=M-1\n"         \
    "D=M\n"            \
    "@%s.%d\n"          \
    "M=D\n"

#define ASM_POP_LATT \
    "@%s\n"          \
    "D=M\n"          \
    "@%d\n"           \
    "D=D+A\n"        \
    "@R13\n"         \
    "M=D\n"          \
    "@SP\n"          \
    "AM=M-1\n"       \
    "D=M\n"          \
    "@R13\n"         \
    "A=M\n"          \
    "M=D\n"

#define ASM_POP_TEMP \
    "@5\n"           \
    "D=A\n"          \
    "@%d\n"          \
    "D=D+A\n"        \
    "@R13\n"         \
    "M=D\n"          \
    "@SP\n"          \
    "AM=M-1\n"       \
    "D=M\n"          \
    "@R13\n"         \
    "A=M\n"          \
    "M=D\n"

#define ASM_POP_POINTER \
    "@SP\n"             \
    "AM=M-1\n"          \
    "D=M\n"             \
    "@%s\n"             \
    "M=D\n"

#define ASM_ADD \
    "@SP\n"     \
    "AM=M-1\n"  \
    "D=M\n"     \
    "A=A-1\n"   \
    "M=D+M\n"

#define ASM_SUB \
    "@SP\n"     \
    "AM=M-1\n"  \
    "D=M\n"     \
    "A=A-1\n"   \
    "M=M-D\n"

#define ASM_NEG \
    "@SP\n"     \
    "A=M-1\n"   \
    "M=-M\n"

#define ASM_AND \
    "@SP\n"     \
    "AM=M-1\n"  \
    "D=M\n"     \
    "A=A-1\n"   \
    "M=D&M\n"

#define ASM_OR  \
    "@SP\n"     \
    "AM=M-1\n"  \
    "D=M\n"     \
    "A=A-1\n"   \
    "M=D|M\n"

#define ASM_NOT \
    "@SP\n"     \
    "A=M-1\n"   \
    "M=!M\n"

#define ASM_EQ      \
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "A=A-1\n"       \
    "D=M-D\n"       \
    "M=-1\n"        \
    "@EQ_LBL_%d\n"  \
    "D;JEQ\n"       \
    "@SP\n"         \
    "A=M-1\n"       \
    "M=0\n"         \
    "(EQ_LBL_%d)\n"

#define ASM_GT      \
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "A=A-1\n"       \
    "D=M-D\n"       \
    "M=-1\n"        \
    "@GT_LBL_%d\n"  \
    "D;JGT\n"       \
    "@SP\n"         \
    "A=M-1\n"       \
    "M=0\n"         \
    "(GT_LBL_%d)\n"

#define ASM_LT      \
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "A=A-1\n"       \
    "D=D-M\n"       \
    "M=-1\n"        \
    "@END\n"        \
    "@LT_LBL_%d\n"  \
    "D;JGT\n"       \
    "@SP\n"         \
    "A=M-1\n"       \
    "M=0\n"         \
    "(LT_LBL_%d)\n"

#define ASM_LABEL   \
    "(%s$%s)\n"

#define ASM_GOTO    \
    "@%s$%s\n"      \
    "0;JMP\n"

#define ASM_IFGOTO  \
    "@SP\n"         \
    "AM=M-1\n"      \
    "D=M\n"         \
    "@%s$%s\n"      \
    "D=D;JNE\n"
