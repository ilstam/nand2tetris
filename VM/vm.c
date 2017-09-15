#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"
#include "exit.h"

#define MAX_LINE_LEN 200
#define MAX_TOKENS 3


typedef enum cmd_id {
    CMD_INVALID = -1,
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
    MAX_COMMANDS  // their total count
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
 * Strip a line from comments and removes new line character.
 */
char *strip_comments(char *s) {
    // sanity checks
    if (s == NULL) {
        return NULL;
    } else if (!*s) {
        return s;
    }

    char *s2 = s;

    for (; *s2; s2++) {
        if (*s2 == '/' && *(s2+1) == '/') {
            *s2 = '\0';
            break;
        }
    }
    if (s2 != s && *(s2-1) == '\n') {
        // remove new line char
        *(s2-1) = '\0';
    }

    return s;
}

typedef bool (*parser_ptr)(int, const char **, char *);

bool parser_push(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_pop(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_add(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_sub(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_neg(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_and(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_or(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_not(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_eq(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_gt(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}
bool parser_lt(__attribute__((unused)) int nargs, __attribute__((unused)) const char *args[nargs], __attribute__((unused)) char *output) {
    return true;
}

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        exit_program(EXIT_MANY_FILES);
    }

    /*
     * Holds current line read.
     */
    char line[MAX_LINE_LEN + 1];
    /*
     * To be filled with command tokens.
     */
    char *tokens[MAX_TOKENS + 1] = {NULL};
    /*
     * Number of tokens of current line / command.
     */
    int ntokens;

    parser_ptr parser_fn[MAX_COMMANDS] = {
        [CMD_PUSH] = parser_push, [CMD_POP] = parser_pop, [CMD_ADD] = parser_add,
        [CMD_SUB] = parser_sub, [CMD_NEG] = parser_neg, [CMD_AND] = parser_and,
        [CMD_OR] = parser_or, [CMD_NOT] = parser_not, [CMD_EQ] = parser_eq,
        [CMD_GT] = parser_gt, [CMD_LT] = parser_lt
    };

    FILE *fp = file_open_or_bail(argv[1], "r");

    while (fgets(line, sizeof(line), fp)) {
        strip_comments(line);
        if (is_empty(line)) {
            continue; // skip empty lines
        }

        ntokens = s_tokenize(line, tokens, MAX_TOKENS+1, " ");
        printf("%d\n", ntokens);
        for (int i = 0; i < ntokens; i++) {
            puts(tokens[i]);
        }
        puts("---------");
    }

    fclose(fp);

    return 0;
}
