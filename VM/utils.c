#include <string.h>
#include <ctype.h>

#include "utils.h"

int s_tokenize(char *s, char *tokens[], int max_toks, const char *delims)
{
    int i;

    /* sanity checks */
    if (s  == NULL || tokens == NULL || delims == NULL
    || !*s || !*delims || max_toks < 1)
        return 0;

    tokens[0] = strtok(s, delims);
    if (tokens[0] == NULL)
        return 0;

    for (i = 1; i < max_toks && (tokens[i] = strtok(NULL, delims)) != NULL; i++) {
    }

    return i;
}

bool s_is_empty(const char *s) {
    if (s == NULL) {
        return true;
    }

    while (isspace(*s)) {
        s++;
    }

    return *s == '\0' ? true : false;
}

char *fname_remove_ext(char *s) {
    char *last_dot = strrchr(s, '.');

    if (last_dot != NULL) {
        *last_dot = '\0';
    }
    return s;
}
