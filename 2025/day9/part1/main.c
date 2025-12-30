#include<stdlib.h>
#include<stdio.h>

// list of corners that can be used
struct corner
{
	long x;
	long y;
	struct corner* next;
};

struct corner *cornerlist = NULL;

int main(int argc, char *argv[])
{	
	long x;
	long y;
	// scanf reads x & y in one go
	while(scanf("%ld%*[,]%ld%*[\n]", &x, &y) != EOF) {

		// Build cornerlist
		struct corner *new = malloc (sizeof *new);
		new->next = cornerlist;
		new->x = x;
		new->y = y;
		cornerlist = new;
	}

	long maxsize = 0;
	// Brute-force is: simply calculate all sizes and find the largest
	for(struct corner *left = cornerlist; left; left = left->next) {
		for(struct corner *right = left->next; right; right=right->next) {
			long width = left->x - right->x;
			long height = left->y - right->y;
			if (width < 0) width = -width;
			if (height < 0) height = -height;
			width++;
			height++;
			long size = width*height;
			if (size > maxsize)
				maxsize = size;
		}

	}
	printf("%ld\n", maxsize);
	return 0;
}
