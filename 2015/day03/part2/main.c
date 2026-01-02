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
	int robox = 0;
	int roboy = 0;
	
	struct house* house = newhouse(0, 0);
	int houseno = 0;
	while ((c = getchar())!= EOF) {
		houseno++;
		if (houseno & 1) {
			switch (c) {
				case '^': robox++; break;
				case 'v': robox--; break;
				case '<': roboy--; break;
				case '>': roboy++; break;
				default: break;
			}
			house = findhouse(robox,roboy);
			if (!house)
				house = newhouse(robox,roboy);
			house->giftcount++;
		}
		else {
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
	}
	int count = 0;
	for(struct house* house = houselist; house; house = house->next)
		count++;
	printf ("Visited houses = %d\n", count);

	return 0;
}

