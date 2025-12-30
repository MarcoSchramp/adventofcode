#include<stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	long from;
	long to;
	long sum  = 0;

	// Read next range
	while (scanf("%ld%*[-]%ld", &from , &to) != EOF) {

		// Skip separator
		getchar(); 

		// Assume that the ranges are managable small to iterate (should have
		// Should have done same approach in previous exercise
		for (long liter = from ; liter <= to; liter++) {
			// A long fits easily in 100 positions
			char buf[100];
			sprintf(buf, "%ld", liter);

			int numdigits = strlen(buf);

			// Try to find pattern with "skip" length
			for (int skip = 1; skip < numdigits; skip++) {

				// Must fit whole times
				if (numdigits % skip)
					continue;

				// Assume match
				int match = 1;
				for (int pos = 0; pos < numdigits - skip; pos++)
					// until proven otherwise
					if(buf[pos+skip] != buf[pos])
						match = 0;

				if (match) {
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
