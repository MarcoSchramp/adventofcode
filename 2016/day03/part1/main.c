
#include <stdio.h>

int main(int argc, char* argv[])
{
	int a;
	int b;
	int c;
	int count = 0;

	while (scanf("%d %d %d", &a, &b, &c) != EOF) 
		if ((a + b > c) && (a + c > b) && (b + c > a)) 
			count++;
	printf ("Correct triangles: %d\n", count);
	return 0;
}
