#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

long countexpansion(char *buffer, int size, int level)
{
	char commandbuffer[20];

	int repeatfill = 0;
	long outputcount = 0;
	for (char * origbuffer = buffer; buffer - origbuffer < size; buffer++) {
		if (*buffer == '(') {
			memset(commandbuffer, 0, sizeof commandbuffer);
			repeatfill = 1;
		}
		else if (repeatfill) {
			if (*buffer != ')') { // buildup the instruction until ')'
				commandbuffer[strlen(commandbuffer)] = *buffer;
			}
			else {
				int repeatsize = atoi(strtok(commandbuffer, "x"));
				int repeatcount = atoi(strtok(NULL, "x"));
				long ocount = countexpansion(buffer + 1, repeatsize, level + 1) * repeatcount;
				outputcount += ocount;
				
				// skip block
				buffer += repeatsize;
				repeatfill = 0;
			}
		}
		else if (!isspace((int) *buffer)) // Why doesn't C do the usual promotion from char to int and must I explictly cast from char to int
			outputcount++; // for verbatim output
	}

	return outputcount;
}

int main(int argc, char* argv[])
{
	char* repeatbuffer = malloc(20000);

	fgets(repeatbuffer, 20000, stdin);
	long count = countexpansion(repeatbuffer, strlen(repeatbuffer), 0);
	printf ("Total expansion size = %ld", count);

	return 0;
}
