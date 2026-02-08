
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[])
{
	char line[1024];
	int count = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// remove trailing newline
		line[strlen(line) - 1] = 0;
	
		// No need to process very short lines, cannot be a match
		if (strlen(line) < 4)
			continue;
	

		int inbrackets = 0;
		int found = 0;
		int foundinbrackets = 0;
		
		for (char *p = line; *(p+3) != 0; p++) {
			if (*p == '[') inbrackets = 1;
			if (*p == ']') inbrackets = 0;
			if (*p == *(p+3) && *(p+1) == *(p+2) && *p != *(p+1)) {
				if (inbrackets) foundinbrackets  = 1;
				else	found = 1;
			}
		}
		if (found && !foundinbrackets)
			count++;
	}
	printf ("Nr of IPs with TLS: %d\n", count);

	return 0;
}
