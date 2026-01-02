#include <stdio.h>

int main (int argc, char* argv[])
{
	int width, height, depth;
	int totallength = 0;

	while (scanf("%d%*[x]%d%*[x]%d%*[\n]", &width, &height, &depth) != EOF) {
		int side1 = width + height;
		int side2 = width + depth;
		int side3 = height + depth;
		int smallest = side1;
		if (smallest > side2) smallest = side2;
		if (smallest > side3) smallest = side3;

		int length = 2 * smallest + width * height * depth;
		totallength += length;

	}
	printf("Total length = %d\n", totallength);
	return 0;
}

