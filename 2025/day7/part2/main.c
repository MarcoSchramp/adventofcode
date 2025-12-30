#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXL 1000

int main(int argc, char* argv[])
{
	char line[MAXL];
	char prevline[MAXL];
	long pathcount[MAXL];
	long prevpathcount[MAXL];
	

	int splitcount = 0;
	fgets(prevline, sizeof line, stdin);
	for (int i = 0; i < MAXL; i++) prevpathcount[i] = 0;
	while (fgets(line, sizeof line, stdin) != NULL) {
		line[strlen(line) - 1] = 0;
		for (int i = 0; i < MAXL; i++) pathcount[i] = 0;

		int length = strlen(line);
		for(int i =0; i < length; i++) {
			if (prevline[i] == 'S' && line[i] == '.') {
				pathcount[i] = 1;
				line[i] = '|';
			}
			if (prevline[i] == '|' && line[i] !='^') {
				line[i] = '|';
				pathcount[i] += prevpathcount[i];
			}
			if (prevline[i] == '|' && line[i] == '^') {
				splitcount++;
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
		printf("%s\n", line);
		strcpy(prevline, line);
		for (int i = 0; i < MAXL; i++) prevpathcount[i] = pathcount[i];
	}
	printf ("splits = %d\n", splitcount);
	long pathcounts = 0;
	for (int i = 0; i < MAXL; i++) pathcounts += pathcount[i];
	printf ("pathcounts = %ld\n", pathcounts);
	return 0;
}
