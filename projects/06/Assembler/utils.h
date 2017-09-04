#pragma once

#define OPCODE_STR "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"

#define OPCODE_TO_BINARY(opcode) \
  (opcode & 0x8000 ? '1' : '0'), \
  (opcode & 0x4000 ? '1' : '0'), \
  (opcode & 0x2000 ? '1' : '0'), \
  (opcode & 0x1000 ? '1' : '0'), \
  (opcode & 0x0800 ? '1' : '0'), \
  (opcode & 0x0400 ? '1' : '0'), \
  (opcode & 0x0200 ? '1' : '0'), \
  (opcode & 0x0100 ? '1' : '0'), \
  (opcode & 0x0080 ? '1' : '0'), \
  (opcode & 0x0040 ? '1' : '0'), \
  (opcode & 0x0020 ? '1' : '0'), \
  (opcode & 0x0010 ? '1' : '0'), \
  (opcode & 0x0008 ? '1' : '0'), \
  (opcode & 0x0004 ? '1' : '0'), \
  (opcode & 0x0002 ? '1' : '0'), \
  (opcode & 0x0001 ? '1' : '0')


/**
 * Wrap standard *alloc functions to check for the return case of NULL.
 * For each function check that we have a valid pointer or bail hard.
 */

void *assembler_malloc(size_t size);
void *assembler_realloc(void *ptr, size_t size);
