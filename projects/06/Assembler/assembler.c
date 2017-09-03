#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "symbol_table.h"
#include "hack_standard.h"
#include "utils.h"
#include "exit.h"

#define MAX_LINE_LEN  200
#define MAX_LABEL_LEN 198
#define INITIAL_ALLOCATED_MEMORY 400


/*
 * Check whether the given path corresponds to a regular file and if that's
 * the case try to open the file using fopen.
 */
FILE *file_open_or_bail(const char *filename, const char *mode)
{
    struct stat path_stat;

    if (stat(filename, &path_stat) != 0) {
        exit_program(EXIT_FILE_DOES_NOT_EXIST, filename);
    }
    if(!S_ISREG(path_stat.st_mode)) {
        exit_program(EXIT_NOT_REGULAR_FILE, filename);
    }

    FILE *fp = fopen(filename, mode);

    if (fp == NULL) {
        exit_program(EXIT_CANNOT_OPEN_FILE, filename);
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

    /*
     * Indicates number of current instruction.
     * We count only real instructions, not empty lines, comments nor labels.
     */
    unsigned instruction_num = 0;
    /*
     * A buffer keeping all real instructions striped from unnecessary symbols.
     */
    char **all_lines;
    /*
     * This indicates for how many lines of instructions we have allocated
     * memory. We store instructions read from the file in memory after
     * striping them from comments and whitespace for later processing.
     * Whenever we hit this limit and need to store more instructions in
     * memory we double this value.
    */
    unsigned allocated_mem = INITIAL_ALLOCATED_MEMORY;
    /*
     * Keeps current line read.
     */
    char line[MAX_LINE_LEN + 1];
    /*
     * Used to temporarily store the label of the current instruction, if
     * the current instruction declares a new label.
     */
    char label[MAX_LABEL_LEN + 1];

    FILE *fp = file_open_or_bail(filename, "r");

    SymbolTable symtab = symtab_init();
    populate_predefined_symbols(symtab);

    all_lines = assembler_malloc(INITIAL_ALLOCATED_MEMORY * sizeof(char *));

    /* First pass */

    while (fgets(line, sizeof(line), fp)) {
        if (instruction_num > MAX_INSTRUCTION) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTION + 1);
        }

        strip_comments_and_whitespace(line);

        if (!*line) {
            // skip empty lines
            continue;
        }

        if (is_label(line, label)) {
            symtab_add(symtab, label, instruction_num);
            continue;
        }

        if (instruction_num == allocated_mem) {
            // We need more memory for storing lines. Double what we already have.
            unsigned tmp = allocated_mem * 2;
            allocated_mem = tmp > MAX_INSTRUCTION ? MAX_INSTRUCTION : tmp;
            all_lines = assembler_realloc(all_lines, allocated_mem * sizeof(char *));
        }

        all_lines[instruction_num] = assembler_malloc(strlen(line) + 1);
        strcpy(all_lines[instruction_num], line);

        instruction_num++;
    }

    fclose(fp);

    // Reduce allocated memory to the amount of memory that we really need
    // for storing all instructions read.
    all_lines = assembler_realloc(all_lines, instruction_num * sizeof(char *));

    /* Second pass */

    for (unsigned i = 0; i < instruction_num; i++) {
        puts(all_lines[i]);
        free(all_lines[i]);
    }

    symtab_destroy(symtab);
    free(all_lines);

    return 0;
}
