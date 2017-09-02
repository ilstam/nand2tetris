#pragma once

#include "hack_standard.h"

#define SYMBOL_NOT_FOUND -1


typedef struct symbol_table *SymbolTable;


/**
 * Initialise a symbol table by allocating memory for it.
 *
 * retval - The newly allocated SymbolTable object.
 */
SymbolTable symtab_init(void);

/**
 * Add a new symbol name - address pair in the symbol table.
 *
 * This does NOT make any checks on the inputs, so you can insert the same
 * name or address more than once (you shouldn't though).
 *
 * If this fail, the program will terminate.
 */
void symtab_add(SymbolTable table, const char *name, unsigned address);

/**
 * Search for a symbol in the symbol table.
 *
 * retval - Symbol's corresponding hack address if symbol exists in the table.
 *          SYMBOL_NOT_FOUND if symbol doesn't exist in the table.
 */
hack_addr symtab_lookup(SymbolTable table, const char *name);

/**
 * Indicate you are complete with the symbol table. Free and delete any
 * remaining internal structures. You must *always* call this function to
 * dispose of the symbol table.
 */
void symtab_destroy(SymbolTable table);
