
#include <stdio.h>

int main(int argc, char* argv[])
{
	int a[3];
	int b[3];
	int c[3];
	int count = 0;

	while (scanf("%d %d %d", a, a + 1, a + 2) != EOF) {
		scanf("%d %d %d", b, b + 1, b + 2);
		scanf("%d %d %d", c, c + 1, c + 2);
		for (int i = 0; i < 3; i++)
			if ((a[i] + b[i] > c[i]) && (a[i] + c[i] > b[i]) && (b[i] + c[i] > a[i])) 
				count++;
	}
	printf ("Correct triangles: %d\n", count);
	return 0;
}
