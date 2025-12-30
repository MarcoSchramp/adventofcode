#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char * argv[])
{

	// readbuffer
	char line[1024];

	// All lines read
	char *lines[1024];

	int linecount  =0 ;

	// Read all lines
	while (fgets(line, sizeof line, stdin) != NULL) {
		lines[linecount] = strdup(line);
		linecount++;
	}

	// determine square size
	int linelength = strlen(lines[0]);

	// prepare sweep neighbours, initialize with 0
	int  **neighbourcount = malloc(sizeof(*neighbourcount)*(linecount + 2));
	for (int i = 0; i < linecount+2; i++)
	{
		neighbourcount[i] = malloc (sizeof (*neighbourcount) * (linelength + 2));
		for (int j = 0; j < linelength + 2; j++)
			neighbourcount[i][j] = 0;
	}

	// Sweep image, increase all neighbours with 1 when '@' found
	for (int i = 0; i < linecount; i++) {
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
	}

	// sweep based on input and neighbourcount to find which rolls have less than 4 neighbours
	int rollcount = 0;
	for (int i = 0; i < linecount; i++) {
		for (int j = 0; j < linelength; j++)
		{
			if (lines[i][j] == '@' && neighbourcount[i+1][j+1] < 4)
				rollcount++;
		}
	}

	printf("Rollcount = %d\n", rollcount);
	return 0;
	
}
