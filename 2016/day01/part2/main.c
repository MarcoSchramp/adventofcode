#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct pos {
	struct pos * next;
	int x;
	int y;
};

struct pos* poslist = NULL;

struct pos* findpos(int x, int y)
{
	for (struct pos* p = poslist; p ; p = p->next)
		if (p->x == x && p->y == y)
			return p;
	return NULL;
}

void addpos(int x, int y)
{
	struct pos* p = malloc(sizeof *p);
	p->x = x;
	p->y = y;
	p->next = poslist;
	poslist = p;
}


int main(int argc, char* argv[])
{
	char line[1024];

	fgets(line, sizeof line, stdin);

	int x = 0;
	int y = 0;

	int dirx = 1;
	int diry = 0;
	addpos(0,0);
	int stepcount = 0;
	for(char *cmd = strtok(line, ", \n"); cmd; cmd = strtok(NULL, ", \n")) {
		stepcount++;
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
		for(int i = 0; i < steps; i++) {
			x += dirx ;
			y += diry;
			if (findpos(x,y)) {
				printf ("eindpositie = %d, %d\n", x, y);
				x = x < 0? -x:x;
				y = y < 0 ? -y : y;
				printf ("Afstand = %d\n", x+y);
				return 0;
			}
			addpos(x,y);
		}
			
	}

	return 0;
}
