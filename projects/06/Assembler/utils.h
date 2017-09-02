#pragma once


/**
 * Wrap standard malloc function to check for the return case of NULL.
 * Check that we have a valid pointer or bail hard.
 */
void *assembler_malloc(size_t size);
