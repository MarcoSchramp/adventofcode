#include <stdlib.h>
#include <stdio.h>

struct house {
	struct house* next;
	int x;
	int y;
	int giftcount;
};

struct house* houselist;

struct house* findhouse(int x, int y)
{
	struct house* house;
	for (house = houselist; house && (house->x != x || house->y != y); house = house->next)
		;
	return house;
}

struct house* newhouse(int x, int y)
{
	struct house* house = malloc(sizeof *house);
	house->x = x;
	house->y = y;
	house->giftcount = 0;
	house->next = houselist;
	houselist = house;
	return house;
}

int main(int argc, char *argv[])
{
	int c;
	int x = 0;
	int y = 0;
	
	struct house* house = newhouse(0, 0);
	while ((c = getchar())!= EOF) {
		switch (c) {
			case '^': x++; break;
			case 'v': x--; break;
			case '<': y--; break;
			case '>': y++; break;
			default: break;
		}
		house = findhouse(x,y);
		if (!house)
			house = newhouse(x,y);
		house->giftcount++;
	}
	int count = 0;
	for(struct house* house = houselist; house; house = house->next)
		count++;
	printf ("Visited houses = %d\n", count);

	return 0;
}

