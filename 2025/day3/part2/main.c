#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// We need to find 12 digits
#define NUM 12

int main(int argc, char*argv[])
{

	// linebuffer
	char line[1000];

	// buffer for keeping solution
	char pos[NUM+1];
	
	// ensure terminating zero
	pos[NUM] = 0;
	long total = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// Trim off EOL
		line[strlen(line) -1] = 0;
		int prevpos = -1;

		int length = strlen(line);
		// We find digit by digits, starting with the first digit
		for (int i =0; i < NUM; i++) {
	
			// We need to start scanning from the reachable position
			int bestpos = length - NUM + i;

			// See if we can find the highest digit left of startposition
			for (int j = bestpos; j > prevpos; j--)
				if (line[j] >= line[bestpos])
					bestpos = j;

			// Add digit to result
			pos[i] = line[bestpos];
			prevpos = bestpos;
		}

		// convert result in pos
		long maxjolts = atol(pos);

		total += maxjolts;
	}
	printf ("Total Jolts = %ld\n", total);
	return 0;
}

