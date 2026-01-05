#include<stdio.h>

#define RACETIME 2503

struct reindeer {
	char name[100];
	int speed;
	int runtime;
	int waittime;
	int points;
	int distance;
};

void getdistance(struct reindeer* reindeer, int time)
{
	int fullsteps = time / (reindeer->runtime + reindeer->waittime);
	int remainingtime = time % (reindeer->runtime + reindeer->waittime);
	
	if (remainingtime > reindeer->runtime) remainingtime = reindeer->runtime;

	reindeer->distance = reindeer->speed * reindeer->runtime * fullsteps + reindeer->speed * remainingtime;
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
		reindeers[reindeercount].points = 0;
		reindeercount++;

	}

	for (int time = 1; time <= RACETIME; time++)
	{
		for (int i = 0; i < reindeercount; i++) 
			getdistance(reindeers + i, time);

		// Find the longest distance
		int maxdistance = -1;
		for (int i = 0; i < reindeercount; i++) 
			maxdistance = reindeers[i].distance > maxdistance ? reindeers[i].distance : maxdistance;

		// Assign points
		for (int i = 0; i < reindeercount; i++) 
			if (reindeers[i].distance == maxdistance) reindeers[i].points++;
	}

	// Find highscore
	int maxpoints = -1;
	for (int i = 0; i < reindeercount; i++) 
			maxpoints = reindeers[i].points > maxpoints ? reindeers[i].points : maxpoints;

	printf ("Max points %d\n", maxpoints);

	return 0;
}

