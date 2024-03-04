#ifndef SHAMIRS_SECRET_SHARING_H
#define SHAMIRS_SECRET_SHARING_H

#include <stdbool.h>


#include "strtok.h"

#ifdef TEST
	#include "CuTest.h"
#endif

/**

@file

@brief Simple library for providing SSS functionality.


*/

/// Seed the random number generator.  MUST BE CALLED before using the library (unless on arc4random() system).
void seed_random(void);

/// Given a secret, `n`, and `t`, create an array of secrets as text strings.
/// By specifying len, you can use a padded string so that the length of the share
//	does not give away the length of the secret
char ** split_string(char * secret, int len, int n, int t, bool random_id);

/// Given a secret, `n`, and `t`, create a list of shares (`\n` separated).
char * generate_share_strings(char * secret, int len, int n, int t, bool random_id);

/// Given a list of shares (`\n` separated without leading whitespace), recreate the original secret.
char * extract_secret_from_share_strings(const char * string);

char * join_strings(char ** shares, int n);

void free_string_shares(char ** shares, int n);

#endif
