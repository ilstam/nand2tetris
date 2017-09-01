#pragma once

#define SYMTAB_NOT_FOUND 0


typedef struct symbol_table *SymbolTable;

SymbolTable symtab_init(void);
void symtab_add(SymbolTable table, const char *name, unsigned address);
unsigned symtab_lookup(SymbolTable table, const char *name);
void symtab_destroy(SymbolTable table);
