
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


#define MAGIC 1364

#define TARGET_X 31
#define TARGET_Y 39

int iswall(int x, int y)
{
	long number = x*x + 3*x + 2*x*y + y + y*y + MAGIC;
	int bitcount = 0;

	while (number) {
		if (number & 1)
			bitcount++;
		number = number >> 1;
	}
	return bitcount & 1;
}

struct cell {
	int x;
	int y;
	int iswall;
	int stepcount;
};

#define BLOCKWIDTH 16
#define BLOCKHEIGHT 16

struct cellblock {
	struct cell* cells[BLOCKWIDTH][BLOCKHEIGHT];
	int x;
	int y;
	struct cellblock* next;
};
struct cellblock* cellblocklist = NULL;

struct cellblock* findcellblock(int x, int y) {
	for (struct cellblock* cellblock = cellblocklist; cellblock; cellblock = cellblock->next) {
		if (cellblock->x <= x && x < cellblock->x + BLOCKWIDTH && cellblock->y <= y && y < cellblock->y + BLOCKHEIGHT)
			return cellblock;
	}
	return NULL;
}

struct cell* findcell(int x, int y)
{
	struct cellblock* cellblock = findcellblock(x,y);
	if (cellblock)
		return cellblock->cells[x - cellblock->x][y-cellblock->y];
	else 
		return NULL;
}

void insertcell(struct cell* cell)
{
	struct cellblock* cellblock = findcellblock(cell->x, cell->y);
	if (!cellblock) {
		cellblock = malloc (sizeof *cellblock);
		cellblock->next = cellblocklist;
		cellblocklist = cellblock;
		cellblock->x = cell->x - (cell->x % BLOCKWIDTH);
		cellblock->y = cell->y - (cell->y % BLOCKHEIGHT);
		memset(cellblock->cells, 0, sizeof cellblock->cells);
	}
	cellblock->cells[cell->x - cellblock->x][cell->y-cellblock->y] = cell;
}

struct celllist {
	struct celllist* next;
	struct cell* cell;
};



void addcelltocelllist(struct celllist** celllist, struct cell* cell)
{
	struct celllist* newcelllist = malloc (sizeof *newcelllist);
	newcelllist->next = *celllist;
	newcelllist->cell = cell;
	*celllist = newcelllist;
}

void freecelllist(struct celllist **root) 
{
	while (*root) {
		struct celllist* cellllist = *root;
		root = &(*root)->next;
		free(cellllist);
	}
}

struct cell* newcell(int x, int y, int stepcount)
{
	if (x == TARGET_X && y == TARGET_Y) {
		// And we're done!
		printf("Stepcound = %d\n", stepcount);
		exit(0);
	}
	struct cell* cell = malloc(sizeof *cell);
	cell->x  = x;
	cell->y  = y;
	cell->iswall = iswall(x, y);
	cell->stepcount = stepcount;
	insertcell(cell);
	return cell;
}

struct celllist* processcelllist(struct celllist *celllist) {
	struct celllist* nextcelllist = NULL;
	while (celllist) {
		struct cell* cell = celllist->cell;
		if (cell->x > 0) {
			struct cell* leftcell = findcell(cell->x - 1, cell->y);
			if (!leftcell)  {
				leftcell = newcell(cell->x - 1, cell->y, cell->stepcount + 1);
				if (!leftcell->iswall)
					addcelltocelllist(&nextcelllist, leftcell);
			}
		}
		if (cell->y > 0) {
			struct cell* upcell = findcell(cell->x, cell->y - 1);
			if (!upcell)  {
				upcell = newcell(cell->x, cell->y - 1, cell->stepcount + 1);
				if (!upcell->iswall)
					addcelltocelllist(&nextcelllist, upcell);
			}
		}
		struct cell* rightcell = findcell(cell->x + 1, cell->y);
		if (!rightcell)  {
			rightcell = newcell(cell->x + 1, cell->y, cell->stepcount + 1);
			if (!rightcell->iswall)
				addcelltocelllist(&nextcelllist, rightcell);
		}
		struct cell* downcell = findcell(cell->x, cell->y + 1);
		if (!downcell)  {
			downcell = newcell(cell->x, cell->y + 1, cell->stepcount + 1);
			if (!downcell->iswall)
				addcelltocelllist(&nextcelllist, downcell);
		}
				
		celllist = celllist->next;
	}
	return nextcelllist;
}
	
int main(int argc, char* argv[])
{
	struct celllist* celllist = NULL;
	struct cell* startcell = newcell(0, 0, 0);
	addcelltocelllist(&celllist, startcell);
	for (;;) {
		struct celllist* newlist = processcelllist(celllist);
		freecelllist(&celllist);
		celllist = newlist;
	}
	return 0;
}

