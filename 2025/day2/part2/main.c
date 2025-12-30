#include<stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	long from;
	long to;
	long sum  = 0;
	while (scanf("%ld%*[-]%ld", &from , &to) != EOF) {
		getchar(); // skip 1 character
		printf("Processing: %ld to %ld\n", from, to);

		for (long liter = from ; liter <= to; liter++) {
			// A long fits easily in 100 positions
			char buf[100];
			sprintf(buf, "%ld", liter);

			int numdigits = strlen(buf);
			for (int skip = 1; skip < numdigits; skip++) {
				/* Must fit whole times */
				if (numdigits % skip)
					continue;
				int match = 1;
				for (int pos = 0; pos < numdigits - skip; pos++)
					if(buf[pos+skip] != buf[pos]) {
						match = 0;
					}
				if (match) {
					printf("match: %s\n", buf);
					sum += liter;
					// Break out: no need to check with other skip values
					break;
				}
				

			}

		}
	}
	printf ("Total = %ld\n", sum);
	return 0;
}
