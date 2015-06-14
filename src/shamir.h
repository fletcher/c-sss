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

/// Seed the random number generator.  MUST BE CALLED before using the library.
void seed_random(void);

/// Given a secret, `n`, and `t`, create a list of shares (`\n` separated).
char * generate_share_strings(char * secret, int n, int t);

/// Given a list of shares (`\n` separated without leading whitespace), recreate the original secret.
/**

	@bug Sometimes there is an error properly extracting the secret
	from the shares, but I can't track down the cause. It's not clear if
	it's related to the random number generator.  It seems to happen more
	often with secrets that consist of complex UTF-8 characters (e.g.
	`ü`). Rerunning the same keys will then most likely give the correct
	answer the next time. On some secret/key combinations, the failure seems
	to occur roughly 10% of the time.

*/
char * extract_secret_from_share_strings(char * string);

#endif
