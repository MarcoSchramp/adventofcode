#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

char *init = "qtetzkpl";

struct path {
	char* pathstring;
	int x;
	int y;
};

struct pathqueue {
	struct path* path;
	struct pathqueue* next;
};

struct pathqueue* pathqueue = NULL;
struct pathqueue** pathqueueend = &pathqueue;

void appendpath(char *pathstring, int x, int y)
{
	struct path* path = malloc (sizeof *path);
	path->pathstring = strdup(pathstring);
	path->x = x;
	path->y = y;

	struct pathqueue *pathqueue = malloc (sizeof *pathqueue);
	pathqueue->path = path;
	pathqueue->next = NULL;
	*pathqueueend = pathqueue;
	pathqueueend = &pathqueue->next;
}

struct path* takepath()
{
	if (!pathqueue) return NULL;

	struct path *path = pathqueue->path;
	struct pathqueue *p = pathqueue;
	pathqueue = pathqueue->next;
	free(p);
	if (pathqueue == NULL)
		pathqueueend = &pathqueue;
	return path;
}
	

void explore(struct path* path)
{
	char* pathbuffer = malloc(strlen(path->pathstring)+2);

	uint8_t hash[16];
	md5String(path->pathstring, hash);
	if ((hash[0] & 0xF0) >> 4 > 10 && path->y - 1 >= 0) {
		sprintf(pathbuffer, "%s%s", path->pathstring, "U");
		appendpath(pathbuffer, path->x, path->y - 1);
	}
	if ((hash[0] & 0x0F)     > 10 && path->y + 1 < 4) {
		sprintf(pathbuffer, "%s%s", path->pathstring, "D");
		appendpath(pathbuffer, path->x, path->y + 1);
	}
	if ((hash[1] & 0xF0) >> 4 > 10 && path->x - 1 >= 0) {
		sprintf(pathbuffer, "%s%s", path->pathstring, "L");
		appendpath(pathbuffer, path->x - 1, path->y);
	}
	if ((hash[1] & 0x0F)      > 10 && path->x + 1 < 4) {
		sprintf(pathbuffer, "%s%s", path->pathstring, "R");
		appendpath(pathbuffer, path->x + 1, path->y);
	}

	free(pathbuffer);
}
	
int main(int argc, char* argv[])
{
	appendpath(init, 0, 0);
	struct path* path;
#ifdef PART2
	while ((path = takepath())) {
		if (path->x == 3 && path->y == 3) 
			printf ("path found path = %s\n", path->pathstring + strlen(init));
#else
	while ((path = takepath()) && (path->x != 3 || path->y != 3)) {
#endif
//		printf ("Exploring path: %s\n", path->pathstring);
		explore(path);
		free(path->pathstring);
		free(path);
	}
#ifdef PART2
#else
	if (path) {
		printf ("shortest path = %s\n", path->pathstring + strlen(init));
	}
	else {
		printf ("No path found\n");
	}
#endif
	return 0;
}

