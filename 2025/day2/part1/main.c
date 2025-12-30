
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	char from[100];
	char to[100];
	long sum  = 0;
	
	// get the range from input 
	while (scanf("%[0123456789]%*[-]%[0123456789]", from , to) != EOF) {
		// Skip one character (the space separator)
		getchar();

		// When the range has an odd number of digits, then it cannot be a "repeat"
		// as repeats must have the same sequence twice (therefore always an even number
		// digits
		if (strlen(from) == strlen(to)&& (strlen(from) % 2)) {
			// Range has odd number of decimals, so can never contain
			// duplicates
			continue;
		}
		else if ((strlen(from) + 1) == strlen(to)) {
			// The range over 2 lengths
			char half[100];

			// using example : 99999 to 100001"
			// using example : 999999 to 1000001"
			// When from has off nr of digits, then start from a very round number
			// based on length of "to" ("100")
			// Otherwise just start from the first digits in the "from" ("999");

			// Assume no more than 12 digits in half
			if (strlen(from) % 2)
				sprintf(half, "%.*s", (int) (strlen(to) /2), "100000000000");
			else
				sprintf(half, "%.*s", (int) (strlen(from) /2), from);

			// Now make thise into long values
			long lfrom = atol(from);
			long lto = atol(to);
			long lhalf = atol(half);
			long ldouble;
			do {
				// contruct possible value
				char d[100];
				sprintf(d,"%ld%ld", lhalf, lhalf);
				ldouble = atol(d);

				// Check if value in range
				if (lfrom  <= ldouble && ldouble <= lto) {
					sum += ldouble;
				}
				lhalf++;
			
			// Stop when the double value exceeds the range
			} while (ldouble < lto);
		}
		else if (strlen(from) != strlen(to)) {
			// Doesn't happen in the set
			// This would happen when the range is very large or
			// very small:  1-9999
			printf("very difficult range from %s to %s\n", from, to);
		
		}
		else if (strlen(from) == strlen(to) && !(strlen(from) %2)) {

			// Range is in 
			// Find the first half of digits
			char half[100];
			sprintf(half, "%.*s", (int) (strlen(from)  /2), from);

			// Get the long values
			long lfrom = atol(from);
			long lto = atol(to);
			long lhalf = atol(half);
			long ldouble;
			do {
				// construct the next possible value
				char d[100];
				sprintf(d,"%ld%ld", lhalf, lhalf);
				ldouble = atol(d);

				// Check if value in range
				if (lfrom  <= ldouble && ldouble <= lto)
					sum += ldouble;
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
