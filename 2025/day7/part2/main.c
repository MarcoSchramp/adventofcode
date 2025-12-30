#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL 1000

int main(int argc, char* argv[])
{
	// Readline buffer
	char line[MAXL];
	// Readline buffer
	char prevline[MAXL];

	// Counts the number of paths found
	long pathcount[MAXL];
	// Counts the number of paths found on the previous line
	long prevpathcount[MAXL];
	


	
	// Read first line, has no splits (only source) 
	// Therefore no processing
	fgets(prevline, sizeof line, stdin);

	// Initialize the counts of the first line
	for (int i = 0; i < MAXL; i++) prevpathcount[i] = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// remove newline
		line[strlen(line) - 1] = 0;

		// initialize target pathcounts
		for (int i = 0; i < MAXL; i++) pathcount[i] = 0;

		int length = strlen(line);
		for(int i =0; i < length; i++) {
			// If line above has source, then this line
			// starts a new path (pathcount = 1)
			if (prevline[i] == 'S' && line[i] == '.') {
				pathcount[i] = 1;
				line[i] = '|';
			}
			// If the line above has a path and this line not a splitter
			// then this line also is a path, with the same pathcount
			// added (could already have a pathcount due to splitter
			// before)!
			if (prevline[i] == '|' && line[i] !='^') {
				line[i] = '|';
				pathcount[i] += prevpathcount[i];
			}

			// if the line above has a path and this line is a splitter
			// then start two paths, each path adds to the pathcount
			// there can already be a pathcount due to falling or
			// other splitter
			if (prevline[i] == '|' && line[i] == '^') {
				if (i - 1 >= 0) {
					pathcount[i-1] += prevpathcount[i];
					line[i-1] = '|';
				}
				if (i + 1 < length) {
					pathcount[i+1] += prevpathcount[i];
					line[i+1] = '|';
				}
			}
		}

		// Update the prev for the next round
		strcpy(prevline, line);
		for (int i = 0; i < MAXL; i++) prevpathcount[i] = pathcount[i];
	}

	// Count the number of paths
	long pathcounts = 0;
	for (int i = 0; i < MAXL; i++)
		pathcounts += pathcount[i];

	printf ("pathcounts = %ld\n", pathcounts);
	return 0;
}
