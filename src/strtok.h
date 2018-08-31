/* This file is only included since MINGW doesn't have strtok_r, so I can't
   compile for Windows without this */

#include <string.h>

char* strtok_rr(
	char *str,
	const char *delim,
	char **nextp);
