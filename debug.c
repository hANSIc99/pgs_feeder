#include "debug.h"

void d_print(char *src)
{

	char *lut[CHAR_MAX] = { 0 };	// look up table for printable equivalents
	// of non-printable characters
	lut['\n'] = "\\n";
	lut['\t'] = "\\t";
	lut['\b'] = "\\b";
	lut['\0'] = "\\0";

	for (char *p = src; *p != 0; p++) {
		if (isprint(*p))
			putchar(*p);
		else
			fputs(lut[(int)*p], stdout);	// puts adds a newline at the end,
		// fputs does not.
	}
	putchar('\n');
}
