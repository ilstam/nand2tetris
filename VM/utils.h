#pragma once

#include <stdbool.h>


/*
 * Check whether or not the given string is consisted of whitespace only.
 *
 * \retval - true if string is only whitespace or NULL, else false.
 */
bool s_is_empty(const char *s);

/*
 * Tokenize a c-string, into variable size tokens.
 *
 * \param s - mutable string
 * \param tokens - array of strings to be filled with c-strings
 * \param max_toks - maximum number of desired tokens
 * \param delims - string consisting of the desired delimeter characters
 * \retval - Number of tokens which s was broke to on success, else 0.
 */
int s_tokenize(char *s, char *tokens[], int max_toks, const char *delims);

/*
 * Remove the extension from the given filename / path.
 * Expect a mutable c-string as input.
 */
char *fname_remove_ext(char *s);
