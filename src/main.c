/*

	main.c -- Template main()

	Copyright © 2015 Fletcher T. Penney. Licensed under the MIT License.

	## The MIT License ##

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>

#include "d_string.h"

#include "shamir.h"


#define kBUFFERSIZE 4096	// How many bytes to read at a time

DString * stdin_buffer() {
	/* Read from stdin and return a GString *
		`buffer` will need to be freed elsewhere */

	char chunk[kBUFFERSIZE];
	size_t bytes;

	DString * buffer = d_string_new("");

	while ((bytes = fread(chunk, 1, kBUFFERSIZE, stdin)) > 0) {
		d_string_append_c_array(buffer, chunk, bytes);
	}

	fclose(stdin);

	return buffer;
}

DString * scan_file(char * fname) {
	/* Read from stdin and return a GString *
		`buffer` will need to be freed elsewhere */

	char chunk[kBUFFERSIZE];
	size_t bytes;

	FILE * file;

	if ((file = fopen(fname, "r")) == NULL ) {
		return NULL;
	}

	DString * buffer = d_string_new("");

	while ((bytes = fread(chunk, 1, kBUFFERSIZE, file)) > 0) {
		d_string_append_c_array(buffer, chunk, bytes);
	}

	fclose(file);

	return buffer;
}


int main( int argc, char** argv ) {

	seed_random();

	if (argc == 4) {
		// Create shares -- "secret"  n  t

		char * secret = argv[1];

		int n = atoi(argv[2]);

		int t = atoi(argv[3]);

		char * shares = generate_share_strings(secret, n, t);

		fprintf(stdout, "%s", shares);

		free(shares);
	} else if (argc == 3) {
		// Read secret from stdin -- n t < cat secret.txt
		DString * secret = stdin_buffer();

		int n = atoi(argv[1]);

		int t = atoi(argv[2]);

		char * shares = generate_share_strings(secret->str, n, t);

		fprintf(stdout, "%s\n", shares);

		free(shares);
		d_string_free(secret, true);
	} else {
		// Read shares from stdin -- < shares.txt
		DString * shares = stdin_buffer();

		char * secret = extract_secret_from_share_strings(shares->str);

		fprintf(stdout, "%s\n", secret);

		free(secret);

		d_string_free(shares, true);
	}

}
