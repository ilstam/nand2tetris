#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"
#include "utils.h"


/*
 * I use a linked list to hold the symbol table, because I was lazy to implement
 * a hash map. So currently lookups to the table take O(n) time. I may improve
 * this in the future by implementing a data structure with faster lookup times.
 *
 * Insertions happen at the back of the list. The rationale behind this is that
 * some predefined symbols are inserted first in the table and we want access to
 * these symbols to be as fast as possible, so we keep them at the front of the
 * list. By keeping a tail pointer we ensure that insertions take O(1) time.
 */


struct table_entry {
    struct table_entry *next;
    char *name;
    hack_addr address;
};

typedef struct table_entry *TableEntry;


struct symbol_table {
   TableEntry head;
   TableEntry tail;
};


SymbolTable symtab_init(void)
{
    SymbolTable table = assembler_malloc(sizeof(struct symbol_table));
    table->head = NULL;
    table->tail = NULL;
    return table;
}

void symtab_add(SymbolTable table, const char *name, hack_addr address)
{
    TableEntry new_entry = assembler_malloc(sizeof(struct table_entry));

    new_entry->name = assembler_malloc(strlen(name) + 1);
    // the following is safe because we just allocated the exact space needed
    strcpy(new_entry->name, name);
    new_entry->address = address;

    if (table->head == NULL) {
        new_entry->next = NULL;
        table->head = new_entry;
        table->tail = new_entry;
    } else {
        table->tail->next = new_entry;
        table->tail = new_entry;
    }
}

hack_addr symtab_lookup(SymbolTable table, const char *name) {
    TableEntry entry = table->head;

    for (; entry != NULL; entry = entry->next) {
        if (!strcmp(entry->name, name)) {
            return entry->address;
        }
    }

    return SYMBOL_NOT_FOUND;
}

void symtab_destroy(SymbolTable table)
{
    TableEntry tmp = NULL;

    for (; table->head != NULL; table->head = tmp) {
        tmp = table->head->next;
        free(table->head->name);
        free(table->head);
    }

    free(table);
}
