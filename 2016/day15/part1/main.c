#include <stdio.h>

struct disk {
	int positions;
	int start;
};

#if 1
struct disk disks[] = {
	{ 17, 1 },
	{ 7, 0 },
	{ 19, 2 },
	{ 5, 0 },
	{ 3, 0 },
	{ 13, 5 }
#if PART2
	, { 11, 0}
#endif
};
#else
struct disk disks[] = {
	{ 5, 4 },
	{ 2, 1 }
};
#endif

int validchain(int time)
{
	int valid = 1;
	for (int i = 0; valid && i < sizeof disks/sizeof *disks; i++)
		valid = valid && (time + i + disks[i].start + 1) % disks[i].positions == 0;
	return valid;
}

int main(int argc, char *argv[])
{
	int i = 0;
	while (!validchain(i)) i++;
	printf ("Success: %d\n", i);
	
	return 0;
}

