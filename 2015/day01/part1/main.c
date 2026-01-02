#include <stdio.h>

int main(int argc, char *argv[])
{
	int c;

	//initial floor = groundfloor
	int floor = 0;

	while ((c = getchar()) != EOF) {
		if (c == '(')
			floor++;
		else if (c == ')')
			floor--;
	}
	printf ("Floor = %d\n", floor);
	return 0;
}
