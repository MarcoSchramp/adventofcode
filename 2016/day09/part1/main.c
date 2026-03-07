#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define REPEATBUFFERSIZE 100
int main(int argc, char* argv[])
{
	char repeatbuffer[REPEATBUFFERSIZE];

	int c;
	int bufferfill;
	int repeatfill;
	int outputcount = 0;
	while ((c = fgetc(stdin)) != EOF) {
		if (bufferfill) 
			bufferfill--; // Ignore in outputcount
		else if (repeatfill) {
			if (c != ')') // buildup the instruction until ')'
				repeatbuffer[strlen(repeatbuffer)] = c;
			else {
				int repeatsize = atoi(strtok(repeatbuffer, "x"));
				int repeatcount = atoi(strtok(NULL, "x"));
				outputcount += repeatcount * repeatsize;
				bufferfill = repeatsize;
				repeatfill = 0;
			}
		}
		else if (c == '(') {
			memset(repeatbuffer, 0, sizeof repeatbuffer);
			repeatfill = 1;
		}
		else if (!isspace(c))
			outputcount++; // for verbatim output
	}
	printf ("Output size = %d\n", outputcount);


	return 0;
}
