#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
			fflush(stdout);
			charcount = 1;
			prevc = c;
		}
	} while (c != EOF);
}	
