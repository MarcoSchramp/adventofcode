#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 50
#define HEIGHT 6

int screen[WIDTH][HEIGHT];

int main (int argc, char* argv[])
{
	memset(screen, 0, sizeof screen);
	char line[1024];

	while (fgets(line, sizeof line, stdin) != NULL) {
		fputs(line, stdout);
		char* cmd = strtok(line, " x=\n");
		if (!strcmp(cmd, "rect")) {
			int x = atoi(strtok(NULL, " x=\n"));
			int y = atoi(strtok(NULL, " x=\n"));
			for (int i =0; i < x; i++)
				for(int j = 0; j < y; j++)
					screen[i][j] = 1;
		}
		else if (!strcmp(cmd, "rotate")) {
			char *direction = strtok(NULL, " x=\n");
			if (!strcmp(direction, "row")) {
				strtok(NULL, " x=\n"); // skip y;
				int y = atoi(strtok(NULL, " x=\n"));
				strtok(NULL, " x=\n"); // skip by;
				int step = atoi(strtok(NULL, " x=\n"));
				int row[WIDTH];

				// prepare rotated version
				for (int i = 0; i < WIDTH; i++)
					row[(i + step) % WIDTH] = screen[i][y];
				// copy into position
				for (int i = 0; i < WIDTH; i++)
					screen[i][y] = row[i];


			}
			else if (!strcmp(direction, "column")) {
				//strtok(NULL, " x=\n"); // skip x; Already filtered out by strtok before
				int x = atoi(strtok(NULL, " x=\n"));
				strtok(NULL, " x=\n"); // skip by;
				int step = atoi(strtok(NULL, " x=\n"));
				int column[HEIGHT];

				// prepare rotated version
				for (int i = 0; i < HEIGHT; i++)
					column[(i + step) % HEIGHT] = screen[x][i];
				// copy into position
				for (int i = 0; i < HEIGHT; i++)
					screen[x][i] = column[i];
			}
			
		}
	}

	int count = 0;
	for (int j = 0; j < HEIGHT; j++) {
		for (int i = 0; i < WIDTH; i++) {
			count += screen[i][j];
			printf ("%c", screen[i][j] ? '#' : ' ');
		}
		printf ("\n");
	}

	printf("nr of pixels on: %d\n", count);
	
	return 0;
}
