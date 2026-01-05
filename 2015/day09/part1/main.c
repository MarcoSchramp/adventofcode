#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCITY 10
#define MAXCITYLENGTH 50
char *cityindex[MAXCITY];
int distances[MAXCITY][MAXCITY];
int citycount = 0;

int getcityindex(char *city)
{
	int i = 0;
	while (i < MAXCITY && cityindex[i] && strcmp(cityindex[i], city))
		i++;
	if (i == MAXCITY) {
		printf ("Too many cities\n");
		exit(-1);
	}
		
	if (!cityindex[i]) {
		cityindex[i] = strdup(city);
		citycount = i + 1;
	}
	return i;
}

int shortest = 1000000;
int shortestpath[MAXCITY];
int currentpath[MAXCITY];

void findshortestpath(int level, int distance, int currentcity, int remaining[MAXCITY])
{
	int found  = 0;
	for (int i = 0; i < citycount; i++) {
		if (remaining[i] == 0) {
			// Lock the current
			remaining[i] = 1;

			currentpath[level] = i;

			int distancein = level == 0 ? 0 : distance+distances[i][currentcity];
			findshortestpath(level + 1, distancein, i, remaining);

			// Free again
			remaining[i] = 0;
			found = 1;
		}
	}
	if (!found ) {
		if (distance < shortest) {
			shortest = distance;
			memcpy(shortestpath, currentpath, sizeof shortestpath);

		}
	}
}
		
		
	

int main(int argc, char* argv[])
{
	char city1[MAXCITYLENGTH];
	char city2[MAXCITYLENGTH];
	int distance;
	
	memset (distances, 0, sizeof distances);
	memset (cityindex, 0, sizeof cityindex);

	while(scanf("%s to %s = %d", city1, city2, &distance) != EOF) {
		int cityindex1 = getcityindex(city1);
		int cityindex2 = getcityindex(city2);
		distances[cityindex1][cityindex2]  = distance;
		distances[cityindex2][cityindex1]  = distance;
	}

	int remaining[MAXCITY];
	memset(remaining, 0, sizeof remaining);
	findshortestpath(0, 0, 0, remaining);

	printf("\nshortestdistance = %d\n", shortest);
	return 0;
}
