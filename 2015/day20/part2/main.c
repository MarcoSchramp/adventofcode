#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSIEVE 10000000L

long long*sieve;

int main()
{
	// Determine primes up to MAXSIEVE
	// Which allows us to check primeness of houses up to MAXSIEVE*MAXSIEVE
	// but for ease, let's start with values op to MAXSIEVE
	sieve = malloc(MAXSIEVE * sizeof *sieve);
	memset(sieve, 0, MAXSIEVE);

	for (int i = 1; i < MAXSIEVE; i++)
	{
		for (int j = i ; j < MAXSIEVE && j <= 50*i; j += i) {
			sieve[j] += 11 *i;
			if (sieve[j] > 29000000L) {
				// printf ("Hr %d => %lld\n", j, sieve[j]);
				break;	
			}
		}
	
	}
	for (int i = 1; i < MAXSIEVE; i++)
	{
		if (sieve[i] > 29000000L) {
			printf ("Found %d => %lld\n", i, sieve[i]);
			break;
		}
	}
	return 0;
}
