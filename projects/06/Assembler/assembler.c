#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "symbol_table.h"
#include "hack_standard.h"
#include "exit.h"

#define MAX_LINE_LEN  200
#define MAX_LABEL_LEN 198

/*
 * We cannot support more than MAX_ADDRESS instructions because we are not
 * able to address them.
 */
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS


/*
 * Try opening a file using fopen and quit the program if this fails. Just a
 * wrapper for the fopen function.
 */
FILE *file_open_or_bail(const char *filename, const char *mode)
{
    FILE *fp = fopen(filename, mode);

    if (fp == NULL) {
        if (errno == 2) {
            exit_program(EXIT_FILE_DOES_NOT_EXIST, filename);
        } else {
            exit_program(EXIT_CANNOT_OPEN_FILE, filename);
        }
    }

    return fp;
}

/*
 * Strip a line from comments and remove *all* whitespace.
 * Comments are indicated by two adjacent / characters.
 *
 * E.g. "  A  = M  // comment " becomes "A=M"
 */
char *strip_comments_and_whitespace(char *s) {
    // sanity checks
    if (s == NULL) {
        return NULL;
    } else if (!*s) {
        return s;
    }

    char s_new[strlen(s) + 1];
    int i = 0;

    for (char *s2 = s; *s2; s2++) {
        if (*s2 == '/' && *(s2+1) == '/') {
            break; // a comment starts from this point on; skip rest of the line
        } else if (!isspace(*s2)) {
            s_new[i++] = *s2;
        }
    }
    s_new[i] = '\0';

    strcpy(s, s_new); // this is safe because s_new is smaller than s

    return s;
}

/**
 * Parse a line and check whether it defines a label. If so, store label's
 * name in the label paremeter and return true. Else return false.
 * If the return value is false, the contents of the label argument shall not
 * be accessed.
 */
bool is_label(const char *line, char *label)
{
    const char *s = line;
    const char *s_end = s + strlen(line) - 1;

    if (*s != '(') {
        return false;
    }
    if (*s_end != ')') {
        return false;
    }

    for (s=s+1; s < s_end; s++) {
        if (*s == '(' || *s == ')') {
            return false;
        }
        *label++ = *s;
    }
    *label = '\0';

    return true;
}

/**
 * Populate some predefined symbols in the symbol table.
 */
void populate_predefined_symbols(SymbolTable table)
{
    struct predef_symbol {
        char name[50+1];
        hack_addr address;
    };

    struct predef_symbol predef_symbols[NUM_PREDEFINED_SYMS] = {
        {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5},
        {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9}, {"R10", 10}, {"R11", 11},
        {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}, {"SCREEN", 16384},
        {"KBD", 24576}, {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3},
        {"THAT", 4},
    };

    for (int i = 0; i < NUM_PREDEFINED_SYMS; i++) {
        struct predef_symbol s = predef_symbols[i];
        symtab_add(table, s.name, s.address);
    }
}


int main(int argc, const char *argv[])
{
    if (argc != 2) {
        exit_program(EXIT_MANY_FILES);
    }

    const char *filename = argv[1];
    char line[MAX_LINE_LEN + 1];
    char label[MAX_LABEL_LEN + 1];
    unsigned instruction_num = 0;

    FILE *fp = file_open_or_bail(filename, "r");

    SymbolTable symtab = symtab_init();
    populate_predefined_symbols(symtab);

    /* First pass */

    while (fgets(line, sizeof(line), fp)) {
        strip_comments_and_whitespace(line);

        if (!*line) {
            // skip empty lines
            continue;
        }

        if (is_label(line, label)) {
            symtab_add(symtab, label, instruction_num);
            continue;
        }

        puts(line);

        instruction_num++;
        if (instruction_num == MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS);
        }
    }

    symtab_print(symtab);

    /* Second pass */
    /* ... */

    fclose(fp);
    symtab_destroy(symtab);

    return 0;
}
