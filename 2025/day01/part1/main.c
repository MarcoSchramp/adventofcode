#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	// simple read buffer
	char line[1024];

	// Initial position of dial = 50
	int arrowpos =  50;
	int zerocount = 0;

	while (fgets(line, sizeof(line), stdin) != NULL) {
		// Find the command (left/right)
		char cmd = line[0];
		// Find the number of positions to move
		int count = atoi(line+1);

		// Move dial (use modulo to deal with round-behaviour of dail
		if (cmd == 'L')
			arrowpos = (arrowpos - count) % 100;
		else if (cmd == 'R')
			arrowpos = (arrowpos + count) % 100;

		// Count when arrow points to 0
		if (arrowpos == 0)
			zerocount++;
	}

	printf("Password = %d\n", zerocount);
	return 0;
}

