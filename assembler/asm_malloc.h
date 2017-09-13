#pragma once

/**
 * Malloc routines that test returns from malloc and friends for the return
 * case of NULL. From within each function check that we have a valid pointer
 * or bail hard.
 */

void *asm_malloc(size_t size);
void *asm_realloc(void *ptr, size_t size);
