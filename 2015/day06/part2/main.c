#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000

char **lights;

void init_lights()
{
	lights = malloc (SIZE * sizeof *lights);
	for (int i =0; i < SIZE; i++) {
		lights[i] = malloc (SIZE * sizeof *lights);
		memset(lights[i], 0, SIZE * sizeof *lights);
	}
}

void turnon_lights(int from_x, int from_y, int to_x, int to_y) {
	for (int x = from_x; x <= to_x; x++)
		for(int y = from_y; y <= to_y; y++)
			lights[x][y]++;
}

void turnoff_lights(int from_x, int from_y, int to_x, int to_y) {
	for (int x = from_x; x <= to_x; x++)
		for(int y = from_y; y <= to_y; y++) 
			lights[x][y] = lights[x][y] > 0 ? lights[x][y]-1 : 0;
}
void toggle_lights(int from_x, int from_y, int to_x, int to_y) {
	for (int x = from_x; x <= to_x; x++)
		for(int y = from_y; y <= to_y; y++)
			lights[x][y] += 2;
}

int count_brightness()
{
	int count = 0;
	for (int x = 0; x < SIZE; x++)
		for(int y = 0; y < SIZE; y++)
			count += lights[x][y];
	return count;
}

int main(int argc, char* argv[])
{
	char line[1024];
	void (*op) (int, int, int, int);

	init_lights();
	while (fgets(line, sizeof line, stdin) != NULL) {
		char *sizetext = NULL;
		if (strstr(line, "turn on ")) {
			sizetext = line + 8;
			op = turnon_lights;
		}
		else if (strstr(line, "turn off ")) {
			sizetext = line + 9;
			op = turnoff_lights;
		}
		else if (strstr(line, "toggle ")) {
			sizetext = line + 7;
			op = toggle_lights;
		}
		else {
			printf("Unknown op\n");
			return 0;
		}
		int from_x, from_y, to_x, to_y;
		sscanf(sizetext, "%d,%d through %d,%d", &from_x, &from_y, &to_x, &to_y);

		op(from_x, from_y, to_x, to_y);
	
	}

	printf("Count of brightness %d\n", count_brightness());
	return 0;
}

