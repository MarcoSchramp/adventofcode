#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	char line[1024];

	fgets(line, sizeof line, stdin);

	int x = 0;
	int y = 0;

	int dirx = 1;
	int diry = 0;
	for(char *cmd = strtok(line, ", \n"); cmd; cmd = strtok(NULL, ", \n")) {
		int newdirx = dirx;
		int newdiry = diry;
		switch (*cmd) {
			case 'L':
				newdirx = -diry;
				newdiry = dirx;
				break;
			case 'R':
				newdirx = diry;
				newdiry = -dirx;
				break;
		}
		dirx = newdirx;
		diry = newdiry;
		int steps = atoi(cmd+1);
		x += dirx * steps;
		y += diry * steps;
	}
	printf ("eindpositie = %d, %d\n", x, y);
	x = x < 0? -x:x;
	y = y < 0 ? -y : y;
	printf ("Afstand = %d", x+y);

	return 0;
}
