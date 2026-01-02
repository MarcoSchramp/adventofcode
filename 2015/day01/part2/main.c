#include <stdio.h>

int main(int argc, char *argv[])
{
	int c;

	//initial floor = groundfloor
	int floor = 0;
	int charcount = 0;

	while ((c = getchar()) != EOF) {
		charcount++;
		if (c == '(')
			floor++;
		else if (c == ')')
			floor--;
		if (floor == -1) {
			printf ("Basement in step %d\n", charcount);
			return 0;
		}
	}
	printf ("Basement never reached\n");
	return 0;
}
