
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[])
{
	int numpos = 0;
	char number[20];

	int c;
	int total = 0;

	while ((c = getchar()) != EOF) {
		if ((c == '-' && numpos == 0) || strchr("0123456789", c) != NULL) {
			number[numpos++] = c;
			number[numpos] = 0;
		}
		else if (numpos > 0) {
			if (strcmp(number, "-")) {
				int n;
				sscanf(number, "%d", &n);
				total += n;
				numpos = 0;
			}
		}
		
	}
	printf("Totaal = %d\n", total);
	return 0;
}
