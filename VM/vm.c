#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>

#include "mapper.h"
#include "utils.h"
#include "exit.h"

#define MAX_LINE_LEN 200
/* Max chars of generated assembly output for a single line/command. */
#define MAX_ASM_OUT  2000
/* Number of tokens of the command with the most tokens (out of all cmds). */
#define MAX_TOKENS 3
#define MAX_FNAME_CHARS 150
/* Max number of vm files that VM translator can handle */
#define MAX_FILES 100
#define MAX_FILENAME_LEN 1000
#define VM_EXTENSION ".vm"

#define PRINT_TO_FILE 0

static unsigned eq_label_counter = 0;
static unsigned gt_label_counter = 0;
static unsigned lt_label_counter = 0;

/* Name of current file being processed without extension. */
char fname_noext[MAX_FNAME_CHARS+1];
/* Full path of output file. */
char path_out[MAX_FNAME_CHARS+1];
/* Name of current function that is being processed. */
char current_fun[MAX_FNAME_CHARS+1];


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
    CMD_LABEL,
    CMD_GOTO,
    CMD_IFGOTO,
    CMD_FUNCTION,
    CMD_RETURN,
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
    } else if (!strcmp(s, "label")) {
        id = CMD_LABEL;
    } else if (!strcmp(s, "goto")) {
        id = CMD_GOTO;
    } else if (!strcmp(s, "if-goto")) {
        id = CMD_IFGOTO;
    } else if (!strcmp(s, "function")) {
        id = CMD_FUNCTION;
    } else if (!strcmp(s, "return")) {
        id = CMD_RETURN;
    }

    return id;
}

/*
 * Strip a line from comments and remove trailing whitespace.
 */
char *strip_comments(char *s)
{
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


bool parser_invalid(__attribute__((unused)) int nargs,
                    __attribute__((unused)) const char *args[nargs],
                    __attribute__((unused)) char *output)
{
    return false;
}

bool parser_push(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 3) {
        return false;
    }

    char *endptr = NULL;
    int i = strtol(args[2], &endptr, 10);

    if (args[2] == endptr || errno != 0 || !args[2] || *endptr || i < 0) {
        return false; // not a number
    }

    if (!strcmp(args[1], "constant")) {
        sprintf(output, ASM_PUSH_CONST, i);
    } else if (!strcmp(args[1], "static")) {
        sprintf(output, ASM_PUSH_STATIC, fname_noext, i);
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

bool parser_pop(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 3) {
        return false;
    }

    char *endptr = NULL;
    int i = strtol(args[2], &endptr, 10);

    if (args[2] == endptr || errno != 0 || !args[2] || *endptr || i < 0) {
        return false; // not a number
    }

    if (!strcmp(args[1], "static")) {
        sprintf(output, ASM_POP_STATIC, fname_noext, i);
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

bool parser_add(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_ADD);
    return true;
}

bool parser_sub(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_SUB);
    return true;
}

bool parser_neg(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_NEG);
    return true;
}

bool parser_and(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_AND);
    return true;
}

bool parser_or(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_OR);
    return true;
}

bool parser_not(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    strcpy(output, ASM_NOT);
    return true;
}

bool parser_eq(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_EQ, eq_label_counter, eq_label_counter);
    eq_label_counter++;
    return true;
}

bool parser_gt(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_GT, gt_label_counter, gt_label_counter);
    gt_label_counter++;
    return true;
}

bool parser_lt(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_LT, lt_label_counter, lt_label_counter);
    lt_label_counter++;
    return true;
}

bool parser_label(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 2) {
        return false;
    }
    sprintf(output, ASM_LABEL, current_fun, args[1]);
    return true;
}

bool parser_goto(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 2) {
        return false;
    }
    sprintf(output, ASM_GOTO, current_fun, args[1]);
    return true;
}

bool parser_ifgoto(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 2) {
        return false;
    }
    sprintf(output, ASM_IFGOTO, current_fun, args[1]);
    return true;
}

bool parser_function(int nargs, const char *args[nargs], char *output)
{
    if (nargs != 3) {
        return false;
    }

    char *endptr = NULL;
    int nvars = strtol(args[2], &endptr, 10);
    char tmp_output[MAX_ASM_OUT+1];

    if (args[2] == endptr || errno != 0 || !args[2] || *endptr || nvars < 0) {
        return false; // not a number
    }

    strcpy(current_fun, args[1]);

    for (int i = 0; i < nvars; i++) {
        parser_push(3, (const char *[]) { "push", "constant", "0" }, tmp_output);
        strncat(output, tmp_output, MAX_ASM_OUT);
        output[MAX_ASM_OUT] = '\0';
    }

    return true;
}

bool parser_return(int nargs, __attribute__((unused)) const char *args[nargs], char *output)
{
    if (nargs != 1) {
        return false;
    }
    sprintf(output, ASM_RETURN);
    return true;
}

/*
 * If path is a directory put in files array all filenames of regular files
 * contained in this directory that end in ".vm", or put path in files if it
 * is a regular file.
 *
 * This function also sets the global path_out.
 *
 * \param path - path to be processed
 * \param maxfiles - Maximum number of files to store in files array.
 * \retval - Number of files that have been put in files array.
 */
int files_to_translate(const char *path, char files[][MAX_FILENAME_LEN+1], int maxfiles)
{
    int num_files = 0;
    bool is_dir = false;
    struct stat path_stat;
    char tmp[MAX_FNAME_CHARS+1];
    char real_path[MAX_FNAME_CHARS+1] = {0};
    char dir_name[MAX_FNAME_CHARS+1] = {0};
    char base_name[MAX_FNAME_CHARS+1] = {0};

    if (stat(path, &path_stat) != 0) {
        exit_program(EXIT_FILE_DOES_NOT_EXIST, path);
    } else if (S_ISREG(path_stat.st_mode)) {
    } else if (S_ISDIR(path_stat.st_mode)) {
        is_dir = true;
    } else {
        exit_program(EXIT_NOT_FILE_OR_DIR, path);
    }

    strcpy(real_path, realpath(path, tmp));
    if (is_dir) {
        strcpy(dir_name, real_path);
    } else {
        strcpy(dir_name, dirname(strcpy(tmp, real_path)));
    }
    strcpy(base_name, basename(strcpy(tmp, real_path)));

    strcpy(path_out, dir_name);
    strcat(path_out, "/");

    if (is_dir) {
        DIR *d;
        struct dirent *dir;
        char *dot, *slash;

        if ((d = opendir(real_path))) {
            while ((dir = readdir(d)) != NULL) {
                dot = strrchr(dir->d_name, '.');

                if (dir->d_type == DT_REG && dot && !strcmp(dot, VM_EXTENSION)) {
                    strcpy(tmp, dir_name);
                    strcat(tmp, "/");
                    strcpy(files[num_files++], strcat(tmp, dir->d_name));
                    if (num_files == maxfiles) {
                        break;
                    }
                }
            }
            closedir(d);
        }

        slash = strrchr(dir_name, '/');
        if (slash == NULL) {
        }
        strcat(path_out, slash+1);
    } else {
        strcpy(files[0], path);
        num_files = 1;

        fname_remove_ext(strcpy(tmp, base_name));
        strcat(path_out, tmp);
    }

    strcat(path_out, ".asm");

    return num_files;
}

int main(int argc, const char *argv[])
{
    /*
     * Number of files to be processed.
     */
    int num_files;
    /*
     * Names of files to be processed.
     */
    char filenames[MAX_FILES][1000+1];
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

    if (argc != 2) {
        exit_program(EXIT_MANY_ARGS);
    }

    strcpy(current_fun, "OutOfFunction");

    parser_ptr parser_fn[MAX_COMMANDS] = {
        [CMD_INVALID] = parser_invalid, [CMD_PUSH] = parser_push,
        [CMD_POP] = parser_pop, [CMD_ADD] = parser_add, [CMD_SUB] = parser_sub,
        [CMD_NEG] = parser_neg, [CMD_AND] = parser_and, [CMD_OR] = parser_or,
        [CMD_NOT] = parser_not, [CMD_EQ] = parser_eq, [CMD_GT] = parser_gt,
        [CMD_LT] = parser_lt, [CMD_LABEL] = parser_label, [CMD_GOTO] = parser_goto,
        [CMD_IFGOTO] = parser_ifgoto, [CMD_FUNCTION] = parser_function,
        [CMD_RETURN] = parser_return
    };

    num_files = files_to_translate(argv[1], filenames, MAX_FILES);

    if (num_files == 0) {
        exit_program(EXIT_NO_FILES_FOUND, path_out);
    }

    #if PRINT_TO_FILE
        FILE *fp_output = fopen(path_out, "w");
        if (fp_output == NULL) {
            exit_program(EXIT_CANNOT_OPEN_FILE_OUT, path_out);
        }
    #endif

    for (int i = 0; i < num_files; i++) {
        FILE *fp_input = fopen(filenames[i], "r");
        if (fp_input == NULL) {
            exit_program(EXIT_CANNOT_OPEN_FILE, filenames[i]);
        }

        strcpy(fname_noext, basename(strcpy(fname_noext, filenames[i])));
        fname_remove_ext(fname_noext);

        while (fgets(line, sizeof(line), fp_input)) {
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

            #if PRINT_TO_FILE
                fprintf(fp_output, asm_output);
            #else
                printf(asm_output);
            #endif
        }

        fclose(fp_input);
    }

    #if PRINT_TO_FILE
        fclose(fp_output);
    #endif

    return 0;
}
