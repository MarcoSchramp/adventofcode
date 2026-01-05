
/* For those who didn't recognize it: the lights follow the same
 * rules as in Conways Game of Life (check wikipedia)
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_GRIDSIZE 1000
#define MAX_ITER 100

char* grid[MAX_GRIDSIZE];
int gridheight = 0;
int gridwidth = 0;

void fixcorners()
{
	grid[0][0] = '#';
	grid[0][gridwidth - 1] = '#';	
	grid[gridheight - 1][0] = '#';
	grid[gridheight -1][gridwidth - 1] = '#';
}

void iterate()
{
	// Sliding window of 3 rows
	fixcorners();

	char* countgrid1 = malloc(gridwidth + 2);
	char* countgrid2 = malloc(gridwidth + 2);
	char* countgrid3 = malloc(gridwidth + 2);
	char* countgrid[3] = { countgrid1, countgrid2, countgrid3 };
	memset(countgrid1, 0, gridwidth + 2);
	memset(countgrid2, 0, gridwidth + 2);
	memset(countgrid3, 0, gridwidth + 2);
	for(int row = 0; row < gridheight + 1; row++) {
		if (row != gridheight) 
			for (int col = 0; col < gridwidth; col++)
			{
				if(grid[row][col] == '#') {
					countgrid[0][col] ++;
					countgrid[0][col+1] ++;
					countgrid[0][col+2] ++;
					countgrid[1][col] ++;
					countgrid[1][col+2] ++;
					countgrid[2][col] ++;
					countgrid[2][col+1] ++;
					countgrid[2][col+2] ++;
				}
			}
		if (row > 0) {
			for (int col = 0; col < gridwidth; col++)
			{
				if(countgrid[0][col + 1] < 2)
					grid[row - 1][col] = '.';
				else if (countgrid[0][col + 1] == 3)
					grid[row - 1][col] = '#';
				else if (countgrid[0][col + 1] > 3)
					grid[row - 1][col] = '.';
			}
		}

		char* tmp = countgrid[0];
		countgrid[0] = countgrid[1];
		countgrid[1] = countgrid[2];
		countgrid[2] = tmp;
		memset(tmp, 0, gridwidth + 2);
	}
	free(countgrid1);
	free(countgrid2);
	free(countgrid3);

	fixcorners();
}


int main(int argc, char* argv[])
{
	char line[1024];

	while(fgets(line, sizeof line, stdin) != NULL) {
		// Overwrite possible newline with zero terminator
		if (strchr(line, '\n'))
			*strchr(line, '\n') = 0;
		grid[gridheight++] = strdup(line);
		if (gridwidth && gridwidth != strlen(line)) {
			printf ("Non-rectangular input!!!\n");
			exit(-1);
		}
		if (!gridwidth) gridwidth = strlen(line);
	}
	for (int i = 0; i < MAX_ITER; i++)
		iterate();

	int count = 0;
	for (int i = 0; i < gridheight; i++) {
		for (int j = 0; j < gridwidth; j++)
			count = grid[i][j] == '#' ? count + 1 : count;
	}

	printf("count = %d", count);
	return 0;
}
