#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <sys/stat.h>

#include "mapper.h"
#include "utils.h"
#include "exit.h"

#define MAX_LINE_LEN 200
/* Max chars of generated assembly output for a single line/command. */
#define MAX_ASM_OUT  1000
/* Number of tokens of the command with the most tokens (out of all cmds). */
#define MAX_TOKENS 3
#define MAX_FNAME_CHARS 150

static unsigned eq_label_counter = 0;
static unsigned gt_label_counter = 0;
static unsigned lt_label_counter = 0;

char filename[MAX_FNAME_CHARS+1];       // name of input file (not full path)
char filename_noext[MAX_FNAME_CHARS+1]; // filename without extension but with .


typedef enum cmd_id {
    CMD_INVALID = 0,
    CMD_PUSH,
    CMD_POP,
    CMD_ADD,
    CMD_SUB,
    CMD_NEG,
    CMD_AND,
    CMD_OR,
    CMD_NOT,
    CMD_EQ,
    CMD_GT,
    CMD_LT,
    MAX_COMMANDS  /* their total count */
} cmd_id;

cmd_id str_to_cmdid(const char *s)
{
    cmd_id id = CMD_INVALID;

    if (s == NULL) {
    } else if (!strcmp(s, "push")) {
        id = CMD_PUSH;
    } else if (!strcmp(s, "pop")) {
        id = CMD_POP;
    } else if (!strcmp(s, "add")) {
        id = CMD_ADD;
    } else if (!strcmp(s, "sub")) {
        id = CMD_SUB;
    } else if (!strcmp(s, "neg")) {
        id = CMD_NEG;
    } else if (!strcmp(s, "and")) {
        id = CMD_AND;
    } else if (!strcmp(s, "or")) {
        id = CMD_OR;
    } else if (!strcmp(s, "not")) {
        id = CMD_NOT;
    } else if (!strcmp(s, "eq")) {
        id = CMD_EQ;
    } else if (!strcmp(s, "gt")) {
        id = CMD_GT;
    } else if (!strcmp(s, "lt")) {
        id = CMD_LT;
    }

    return id;
}

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
 * Strip a line from comments and remove trailing whitespace.
 */
char *strip_comments(char *s) {
    // sanity checks
    if (s == NULL) {
        return NULL;
    } else if (!*s) {
        return s;
    }

    char *s2 = s;
    char *last_char = s;

    for (; *s2; s2++) {
        if (*s2 == '/' && *(s2+1) == '/') {
            *s2 = '\0';
            break;
        } else if (!isspace(*s2)) {
            last_char = s2;
        }
    }

    if (*last_char) {
        *(last_char + 1) = '\0';
    }

    return s;
}

typedef bool (*parser_ptr)(int, const char **, char *);


bool parser_invalid(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return false;
}

bool parser_push(int nargs, const char *args[nargs], char *output) {
    if (nargs != 3) {
        return false;
    }

    char *endptr = NULL;
    int i = strtol(args[2], &endptr, 10);

    if (args[2] == endptr || errno != 0 || !args[2] || *endptr || i < 0) {
        return false; // not a number
    }

    if (!strcmp(args[1], "constant")) {
        sprintf(output, ASM_PUSH_CONST_STATIC, "", i);
    } else if (!strcmp(args[1], "static")) {
        sprintf(output, ASM_PUSH_CONST_STATIC, filename_noext, i);
    } else if (!strcmp(args[1], "local")) {
        sprintf(output, ASM_PUSH_LATT, i, "LCL");
    } else if (!strcmp(args[1], "argument")) {
        sprintf(output, ASM_PUSH_LATT, i, "ARG");
    } else if (!strcmp(args[1], "this")) {
        sprintf(output, ASM_PUSH_LATT, i, "THIS");
    } else if (!strcmp(args[1], "that")) {
        sprintf(output, ASM_PUSH_LATT, i, "THAT");
    } else if (!strcmp(args[1], "temp")) {
        if (i < 1 || i > 8) {
            return false;
        }
        sprintf(output, ASM_PUSH_TEMP, i);
    } else if (!strcmp(args[1], "pointer")) {
        if (i == 0) {
            sprintf(output, ASM_PUSH_POINTER, "THIS");
        } else if (i == 1) {
            sprintf(output, ASM_PUSH_POINTER, "THAT");
        } else {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool parser_pop(int nargs, const char *args[nargs], char *output) {
    if (nargs != 3) {
        return false;
    }

    char *endptr = NULL;
    int i = strtol(args[2], &endptr, 10);

    if (args[2] == endptr || errno != 0 || !args[2] || *endptr || i < 0) {
        return false; // not a number
    }

    if (!strcmp(args[1], "static")) {
        sprintf(output, ASM_POP_STATIC, filename_noext, i);
    } else if (!strcmp(args[1], "local")) {
        sprintf(output, ASM_POP_LATT, "LCL", i);
    } else if (!strcmp(args[1], "argument")) {
        sprintf(output, ASM_POP_LATT, "ARG", i);
    } else if (!strcmp(args[1], "this")) {
        sprintf(output, ASM_POP_LATT, "THIS", i);
    } else if (!strcmp(args[1], "that")) {
        sprintf(output, ASM_POP_LATT, "THAT", i);
    } else if (!strcmp(args[1], "temp")) {
        if (i < 1 || i > 8) {
            return false;
        }
        sprintf(output, ASM_POP_TEMP, i);
    } else if (!strcmp(args[1], "pointer")) {
        if (i == 0) {
            sprintf(output, ASM_POP_POINTER, "THIS");
        } else if (i == 1) {
            sprintf(output, ASM_POP_POINTER, "THAT");
        } else {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

bool parser_add(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_ADD);
    return true;
}

bool parser_sub(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_SUB);
    return true;
}

bool parser_neg(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_NEG);
    return true;
}

bool parser_and(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_AND);
    return true;
}

bool parser_or(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_OR);
    return true;
}

bool parser_not(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_NOT);
    return true;
}

bool parser_eq(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_EQ, eq_label_counter, eq_label_counter);
    eq_label_counter++;
    return true;
}

bool parser_gt(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_GT, gt_label_counter, gt_label_counter);
    gt_label_counter++;
    return true;
}

bool parser_lt(int nargs, __attribute__((unused)) const char *args[nargs], char *output) {
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_LT, lt_label_counter, lt_label_counter);
    lt_label_counter++;
    return true;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        exit_program(EXIT_MANY_FILES);
    }

    // store filename and filename_noext globals
    strcpy(filename, basename(strncpy(filename, argv[1], MAX_FNAME_CHARS)));
    filename[MAX_FNAME_CHARS] = '\0';
    strcat(fname_remove_ext(strcpy(filename_noext, filename)), ".");

    /*
     * Holds current line read.
     */
    char line[MAX_LINE_LEN + 1];
    char tmp_line[MAX_LINE_LEN + 1];
    /*
     * Indicates current file line that is being processed.
     */
    unsigned line_num = 0;
    /*
     * Holds the generated assembly code for the current line read.
     */
    char asm_output[MAX_ASM_OUT + 1];
    /*
     * To be filled with command tokens.
     */
    char *tokens[MAX_TOKENS + 1] = {NULL};
    /*
     * Number of tokens of current line / command.
     */
    int ntokens;

    parser_ptr parser_fn[MAX_COMMANDS] = {
        [CMD_INVALID] = parser_invalid, [CMD_PUSH] = parser_push,
        [CMD_POP] = parser_pop, [CMD_ADD] = parser_add, [CMD_SUB] = parser_sub,
        [CMD_NEG] = parser_neg, [CMD_AND] = parser_and, [CMD_OR] = parser_or,
        [CMD_NOT] = parser_not, [CMD_EQ] = parser_eq, [CMD_GT] = parser_gt,
        [CMD_LT] = parser_lt
    };

    FILE *fp = file_open_or_bail(argv[1], "r");

    while (fgets(line, sizeof(line), fp)) {
        line_num++;

        strip_comments(line);

        if (s_is_empty(line)) {
            continue; // skip empty lines
        }

        strcpy(tmp_line, line);
        ntokens = s_tokenize(tmp_line, tokens, MAX_TOKENS+1, " ");
        // ntokens should be at least 1 because we have skipped empty lines
        cmd_id cmdid = str_to_cmdid(tokens[0]);
        bool valid = parser_fn[cmdid](ntokens, (const char **) tokens, asm_output);

        if (!valid) {
            exit_program(EXIT_INVALID_COMMAND, line_num, line);
        }

        printf(asm_output);
    }

    fclose(fp);

    return 0;
}
