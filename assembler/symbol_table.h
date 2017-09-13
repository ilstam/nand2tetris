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
 * Add a new symbol name-address pair in the symbol table.
 *
 * WARNING: This does NOT check whether the symbol already exist in the table,
 * so it is possible to insert the same symbol twice (which you shall not do).
 *
 * If this fail, the program will terminate.
 */
void symtab_add(SymbolTable table, const char *name, hack_addr address);

/**
 * Search for a symbol in the symbol table.
 *
 * retval - Symbol's corresponding hack address if symbol exists in the table.
 *          SYMBOL_NOT_FOUND if symbol doesn't exist in the table.
 */
hack_addr symtab_lookup(SymbolTable table, const char *name);

/**
 * Translate a symbol to a hack address.
 *
 * If symbol already exists in the symbol table it just returns its corre-
 * sponding hack address. If symbol does not exist in the symbol table, it
 * adds it in the table first and returns its newly assigned address.
 *
 * retval - hack address that corresponds to the given symbol
 */
hack_addr symtab_resolve(SymbolTable table, const char *name);

/**
 * Indicate you are complete with the symbol table. Free and delete any
 * remaining internal structures. You must *always* call this function to
 * dispose of the symbol table.
 */
void symtab_destroy(SymbolTable table);

/**
 * Display all symbols along with their corresponding addresses.
 */
void symtab_print(SymbolTable table);
