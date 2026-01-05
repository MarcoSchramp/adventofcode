#include<stdio.h>

#define RACETIME 2503

struct reindeer {
	char name[100];
	int speed;
	int runtime;
	int waittime;
};

int getdistance(struct reindeer* reindeer, int time)
{
	int fullsteps = time / (reindeer->runtime + reindeer->waittime);
	int remainingtime = time % (reindeer->runtime + reindeer->waittime);
	
	if (remainingtime > reindeer->runtime) remainingtime = reindeer->runtime;

	return reindeer->speed * reindeer->runtime * fullsteps + reindeer->speed * remainingtime;
}

int main(int argc, char* argv[])
{
	char line[1024];
	struct reindeer reindeers[10];
	int reindeercount;

	while (fgets(line, sizeof line, stdin) != NULL)
	{
		sscanf(line, "%s can fly %d km/s for %d seconds, but then must rest for %d",
			reindeers[reindeercount].name,
			&reindeers[reindeercount].speed,
			&reindeers[reindeercount].runtime,
			&reindeers[reindeercount].waittime);
			reindeercount++;

	}
	int maxdist = 0;

	for (int i = 0; i < reindeercount; i++) {
		int dist = getdistance(reindeers + i, RACETIME);
		if (dist > maxdist)
			maxdist = dist;
	}

	printf ("Max distance %d\n", maxdist);

	return 0;
}

