#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
	char line[1000];
	char prevline[1000];
	int splitcount = 0;

	// read a first line , but don't process it
	fgets(prevline, sizeof line, stdin);

	// read next line and compare to previous
	while (fgets(line, sizeof line, stdin) != NULL) {
		// Remove new line
		line[strlen(line) - 1] = 0;

		int length = strlen(line);
		for(int i =0; i < length; i++) {
			// from S and next empty --> falling
			if (prevline[i] == 'S' && line[i] == '.')
				line[i] = '|';
			// If previous is falling and next empty --> next is falling
			if (prevline[i] == '|' && line[i] =='.')
				line[i] = '|';
			// previous if falling on top of splitter --> falling on left and right 
			// & one more split
			if (prevline[i] == '|' && line[i] == '^') {
				splitcount++;
				if (i - 1 >= 0) line[i-1] = '|';
				if (i + 1 < length) line[i+1] = '|';
			}
			// A splitter without anything falling, doesn't work
		}

		// Prepare for next round
		strcpy(prevline, line);
	}
	printf ("splits = %d\n", splitcount);
	return 0;
}
