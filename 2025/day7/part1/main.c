#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
	char line[1000];
	char prevline[1000];
	int splitcount = 0;

	fgets(prevline, sizeof line, stdin);
	while (fgets(line, sizeof line, stdin) != NULL) {
		line[strlen(line) - 1] = 0;

		int length = strlen(line);
		for(int i =0; i < length; i++) {
			if (prevline[i] == 'S' && line[i] == '.')
				line[i] = '|';
			if (prevline[i] == '|' && line[i] =='.')
				line[i] = '|';
			if (prevline[i] == '|' && line[i] == '^') {
				splitcount++;
				if (i - 1 >= 0) line[i-1] = '|';
				if (i + 1 < length) line[i+1] = '|';
			}
		}
		printf("%s\n", line);
		strcpy(prevline, line);
	}
	printf ("splits = %d\n", splitcount);
	return 0;
}
