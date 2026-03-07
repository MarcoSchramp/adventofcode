
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if 1
#ifdef PART2
#define TARGETLENGTH 35651584
#else
#define TARGETLENGTH  272
#endif
char input[] = "00111101111101000";
#else
#define TARGETLENGTH  20
char input[] = "10000";
#endif


char* doublepattern(char* p)
{
	int len = strlen(p);
	int targetbufferlen = len*2 + 2; // one for a zero and one for terminating zero
	char* target = malloc(targetbufferlen * sizeof (*target));
	strcpy(target, p);
	sprintf (target + len, "0");
	for (int i = 0; i < len; i++)
		target[len + 1 + i] = p[len - i - 1] == '1' ? '0' : '1';
	target[len+len+1] = 0;
	return target;
}

void reducepattern(char*p)
{
	char *target = p;
	while (*p) {
		if (p[0] == p[1])
			*target = '1';
		else
			*target = '0';
		p +=2 ;
		target++;
	}
	// Terminate the result
	*target = 0;
}

int main(int argc, char* argv[])
{
	char *pattern = input;
	// 
	while(strlen(pattern) < TARGETLENGTH) {
		pattern = doublepattern(pattern);
	}
	// Cut of string
	pattern[TARGETLENGTH] = 0;

	while (strlen(pattern) % 2 == 0) {
		reducepattern(pattern);
	}
	printf("Checksum =  %s\n", pattern);

	return 0;
}
