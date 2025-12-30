#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char*argv[])
{
	// linebuffer
	char line[1024];
	int total = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {

		// Trim off EOL
		line[strlen(line) -1] = 0;
		int maxjolts = 0;

		int linelength = strlen(line);

		// Just brute-force it: find a combination of 2 that has highest value
		for (int i = 0; i < linelength -1 ; i++)
				for (int j = i + 1; j < linelength; j++) {
					char num[3];
					num[0] = line[i];
					num[1] = line[j];
					num[2] = 0;
					int jolts = atoi(num);
					if (jolts > maxjolts)
						maxjolts = jolts;
				}
		total += maxjolts;
	}

	printf ("Total Jolts = %d\n", total);
	return 0;
}

