#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Done some analysis on the input and found a few things
 * that we base this solution on:
 * 1) There are tree types of expansion: A =? BC and A=> BRnCAr and A=> BRnCYDAr
 *    where B,C and D are again sequences of Atoms.
 * 2) Rn, Ar and Y don't have an expansion, so these can be treated as separators
 * 3) Elements are one capital letter, followed by an optional lower case
 * 4) The solution only needs to know how many steps to produce (or the
 *    other way around, how many steps to recude from the long one)
 * 5) We will assume that there is a solution
 */

#define ARTOKEN  1
#define RNTOKEN  2
#define YTOKEN   3
#define OTOKEN   4
#define EOFTOKEN 5

int gettoken()
{
	int c = getchar();
	if (c == EOF)
		return EOFTOKEN;
	int c1 = getchar();
	// Check for lowercase letters, I don't want to include ctype.h
	if (!strchr("abcdefghijklmnopqrstuvwxyz", c1)) {
		ungetc(c1, stdin);
		c1 = 0;
	}
	if (c == 'A' && c1 == 'r') return ARTOKEN;
	if (c == 'R' && c1 == 'n') return RNTOKEN;
	if (c == 'Y' && c1 == 0)   return YTOKEN;
	if (c == '\n' && c1 == 0)   return EOFTOKEN;
	return OTOKEN;
	
}

int processmolecule() {
	int reductioncount = 0;
	while (1) {
		int token  = gettoken();
		switch(token) {
		case EOFTOKEN: return reductioncount;
		case YTOKEN:   reductioncount -= 1; break;
		case ARTOKEN:  return reductioncount;
		case RNTOKEN:  reductioncount += processmolecule(); break;
		case OTOKEN:   reductioncount += 1;
		}
	}
}

int main(int arcg, char* argv[]) {

	char line[1024];

	while (fgets(line, sizeof line, stdin) != NULL)
		if (!strcmp(line, "\n")) break;
	printf ("Steps: %d\n", processmolecule() - 1);
	return 0;
}
