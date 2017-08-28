#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN  200


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

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        printf("%s: one and only one file operand is expected.\n", __FILE__);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    char line[MAX_LINE_LEN + 1];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("%s: Can't open file %s\n", __FILE__, filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fp)) {
        strip_comments_and_whitespace(line);
        // Note: Maybe check for illegal characters as well here?
        if (*line) {
            puts(line);
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}
