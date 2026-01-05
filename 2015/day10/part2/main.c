#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * this is the first time I didn't use C alone
 * this program does only one iteration, but uses minimal
 * memory to do so (yes, I've declared 3 variables only
 * 
 * The trick is the unix-shell: make the output the input for a next round.
 * so
 *     # a.out < input.txt | a.out | a.out | a.out | a.out
 * runs 5 iterations. You can then run this througc unix wc 
 * to find the number of characters
 */

int main(int argc, char* argv[])
{
	int c;
	int prevc;

	// We read at least 1 char
	int charcount  = 1;
	prevc = getchar();
	do {
		c = getchar();
		if (c == '\n') continue;
		if (c == prevc)
			charcount++;
		else {
			printf("%d%c", charcount, prevc);
			charcount = 1;
			prevc = c;
		}
	} while (c != EOF);
}	
