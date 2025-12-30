#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char * argv[])
{

	// Line buffer
	char line[1024];

	// All lines read
	char *lines[1024];
	int totalremoved = 0;

	int linecount  =0 ;

	// Read all lines
	while (fgets(line, sizeof line, stdin) != NULL) {
		lines[linecount] = strdup(line);
		linecount++;
	}

	// determine width
	int linelength = strlen(lines[0]);

	// setup sweep matrix
	int  **neighbourcount = malloc(sizeof(*neighbourcount)*(linecount + 2));
	for (int i = 0; i < linecount+2; i++)
			neighbourcount[i] = malloc (sizeof (*neighbourcount) * (linelength + 2));
	int rollcount;
	do {
		rollcount = 0;

		// Initialize neighbourcount to 0
		for (int i = 0; i < linecount+2; i++)
			for (int j = 0; j < linelength + 2; j++)
				neighbourcount[i][j] = 0;

		// Sweep the neighbours
		for (int i = 0; i < linecount; i++)
			for (int j = 0; j < linelength; j++)
				if (lines[i][j] == '@')
				{
					neighbourcount[i]   [j]  ++;
					neighbourcount[i+1] [j]  ++;
					neighbourcount[i+2] [j]  ++;
					neighbourcount[i]   [j+1]++;
					neighbourcount[i+2] [j+1]++;
					neighbourcount[i]   [j+2]++;
					neighbourcount[i+1] [j+2]++;
					neighbourcount[i+2] [j+2]++;
				}

		// Elimininate accessible rolls
		for (int i = 0; i < linecount; i++)
			for (int j = 0; j < linelength; j++)
				if (lines[i][j] == '@' && neighbourcount[i+1][j+1] < 4) {
					rollcount++;
					lines[i][j] = '.';
				}
		totalremoved +=rollcount;
	} while (rollcount > 0);
	printf("Total removed = %d\n", totalremoved);
	return 0;
}
