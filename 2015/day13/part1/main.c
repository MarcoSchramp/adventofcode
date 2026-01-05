#include<stdio.h>
#include<string.h>

#define MAX_PERSON 10
#define MAX_NAMELENGTH 30

char *names[MAX_PERSON];
int likes[MAX_PERSON][MAX_PERSON];
int nr_of_persons = 0;

int findpersonindex(char *name)
{
	int i = 0;
	while (names[i] && strcmp(names[i], name))
		i++;

	// Append at end if not found
	if (names[i] == NULL) {
		names[i] = strdup(name);
		nr_of_persons++;
	}
	return i;
}

int optimal = 0;

void findoptimal(int level, int positions[MAX_PERSON], int remaining[MAX_PERSON])
{
	if (level == nr_of_persons) {
		// Start counting
		int totallikes = 0;
		for (int i = 0; i < nr_of_persons; i++) {
			totallikes += likes[positions[i]][positions[(i - 1 + nr_of_persons) % nr_of_persons]];
			totallikes += likes[positions[i]][positions[(i +1) % nr_of_persons]];
		}
		if (totallikes > optimal)
			optimal = totallikes;

	}
	else {
		for (int i = 0 ;i < nr_of_persons; i++) {
			if(!remaining[i]) {
				remaining[i] = 1;
				positions[level] = i;
				findoptimal(level + 1, positions, remaining);
				remaining[i] = 0;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	
	memset(likes, 0, sizeof likes);
	memset(names, 0, sizeof names);

	char name1[MAX_NAMELENGTH];
	char name2[MAX_NAMELENGTH];
	char winlose[MAX_NAMELENGTH];
	int amount;
	while(scanf("%s would %s %d happiness units by sitting next to %s", name1, winlose, &amount, name2) != EOF)
	{
		name2[strlen(name2)-1] = 0;

		// strip last character
		int from = findpersonindex(name1);
		int to = findpersonindex(name2);
		amount = !strcmp(winlose, "gain")? amount : - amount;
		likes[from][to] = amount;
	}

	int positions[MAX_PERSON];
	int remaining[MAX_PERSON];
	memset(remaining, 0 , sizeof remaining);
	findoptimal(0, positions, remaining);

	printf("Optimal = %d\n", optimal);

	return 0;
}

