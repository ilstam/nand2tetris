#pragma once

/**
 * Wrap standard *alloc functions to check for the return case of NULL.
 * For each function check that we have a valid pointer or bail hard.
 */

void *assembler_malloc(size_t size);
void *assembler_realloc(void *ptr, size_t size);
