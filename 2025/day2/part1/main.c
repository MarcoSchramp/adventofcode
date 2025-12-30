
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	char from[100];
	char to[100];
	long sum  = 0;
	while (scanf("%[0123456789]%*[-]%[0123456789]", from , to) != EOF) {
		getchar(); // skip 1 character
		if (strlen(from) == strlen(to)&& (strlen(from) % 2)) {
			// Range has odd number of decimals, so can never contain
			// duplicates
			printf("Skipping range from %s to %s\n", from, to);
			continue;
		}
		else if ((strlen(from) + 1) == strlen(to)) {
			printf("difficult range from %s to %s\n", from, to);
		
			char half[100];
			if (strlen(from) % 2)
				sprintf(half, "%.*s", (int) (strlen(to) /2), "100000000000");
			else
				sprintf(half, "%.*s", (int) (strlen(from) /2), from);
			long lfrom = atol(from);
			long lto = atol(to);
			long lhalf = atol(half);
			long ldouble;
			do {
				char d[100];
				sprintf(d,"%ld%ld", lhalf, lhalf);
				ldouble = atol(d);
				if (lfrom  <= ldouble && ldouble <= lto) {
					printf("Found: %s\n", d);
					sum += ldouble;
				}
				lhalf++;
	
			} while (ldouble < lto);
		}
		else if (strlen(from) != strlen(to)) {
			// Doesn't happen
			printf("very difficult range from %s to %s\n", from, to);
		
		}
		else if (strlen(from) == strlen(to) && !(strlen(from) %2)) {
			printf("processing range from %s to %s\n", from, to);
			char half[100];
			sprintf(half, "%.*s", (int) (strlen(from)  /2), from);
			long lfrom = atol(from);
			long lto = atol(to);
			long lhalf = atol(half);
			long ldouble;
			do {
				char d[100];
				sprintf(d,"%ld%ld", lhalf, lhalf);
				ldouble = atol(d);
				if (lfrom  <= ldouble && ldouble <= lto) {
					printf("Found: %s\n", d);
					sum += ldouble;
				}
				lhalf++;
	
			} while (ldouble < lto);
			
		}
		else {
			// Doesn't happen
			printf("huhhh range from %s to %s\n", from, to);
		}
	}
	printf ("Total = %ld\n", sum);
	return 0;
}
