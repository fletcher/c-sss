#ifndef SHAMIRS_SECRET_SHARING_H
#define SHAMIRS_SECRET_SHARING_H

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

/// Given a secret, `n`, and `t`, create a list of shares (`\n` separated).
char * generate_share_strings(char * secret, int n, int t);

/// Given a list of shares (`\n` separated without leading whitespace), recreate the original secret.
char * extract_secret_from_share_strings(const char * string);

#endif
