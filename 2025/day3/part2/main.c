#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 12
int main(int argc, char*argv[])
{
	char line[1000];
	char pos[NUM+1];
	pos[NUM] = 0;
	long total = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// Trim off EOL
		line[strlen(line) -1] = 0;
		int prevpos = -1;

		int length = strlen(line);
		for (int i =0; i < NUM; i++) {
			int bestpos = length - NUM + i;
			for (int j = bestpos; j > prevpos; j--)
				if (line[j] >= line[bestpos])
					bestpos = j;
			pos[i] = line[bestpos];
			prevpos = bestpos;
		}
		long maxjolts = atol(pos);

		printf ("Jolts = %ld\n", maxjolts);
		total += maxjolts;
	}
	printf ("Total Jolts = %ld\n", total);
	return 0;
}

