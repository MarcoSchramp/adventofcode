#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"marco.h"
struct neigbour;

struct junction {
	int linenr;
	long x;
	long y;
	long z;
	struct junction *next;
	struct neighbour *neighbours;
	int groupid;
};

struct neighbour {
	struct junction* junction;
	struct neighbour *next;
};

typedef struct line {
	struct junction* from;
	struct junction* to;
	long distance;
	struct line *next;
} line_t;

struct junction* junctionlist = NULL;
struct line* linelist = NULL;

void addneighbour(struct junction* j, struct junction* n)
{
	struct neighbour * new = malloc(sizeof (*new));
	new->next = j->neighbours;
	new->junction = n;
	j->neighbours = new;
}

int line_cmp(line_t* left, line_t* right)
{
	return left->distance < right->distance;
}


MERGESORT(line_t, line_cmp)

int taint(struct junction *j, int id)
{
	if (j->groupid == id) return 0;
	printf("Enter taint(%ld, %ld, %ld)\n", j->x, j->y, j->z);
	int taintcount = 1;
	j->groupid = id;
	for (struct neighbour *n = j->neighbours; n; n = n->next)
		taintcount += taint(n->junction, id);
	return taintcount;
}

int main(int argc, char* argv[])
{
	long x;
	long y;
	long z;	
	long junctioncount = 0;
	struct junction ** j =  &junctionlist;

	while (scanf("%ld%*[,]%ld%*[,]%ld%*[^\n]", &x, &y, &z) != EOF) {
		
		*j = malloc(sizeof **j);
		(*j)->linenr = junctioncount + 1;
		(*j)->x = x;
		(*j)->y = y ;
		(*j)->z = z;
		(*j)->next = NULL;
		(*j)->neighbours = NULL;
		(*j)->groupid = 0;
		j = &(*j)->next;
		junctioncount++;
	}

	for (struct junction *i = junctionlist ; i; i = i->next)
		for (struct junction *j = i->next; j; j = j->next) {
			line_t *l = malloc(sizeof *l);
			l->from = i;
			l->to = j;
			l->distance = (i->x - j->x)*(i->x - j->x) +
					 (i->y - j->y)*(i->y - j->y) +
					 (i->z - j->z)*(i->z - j->z);
			l->next = linelist;
			linelist = l;
		}
	linelist = line_t_sortlist(linelist);

	long linkedjunctioncount = 0;
	int taintid = 1;
	struct line *line = linelist;
	while (line ) {
		if ((line->from->groupid != line->to->groupid) ||
		   (line->from->groupid == 0 && line->to->groupid == 0)) {
			printf ("%ld: Connecting (%ld,%ld,%ld) to (%ld,%ld,%ld) dist %ld\n",
				linkedjunctioncount,
				line->from->x, line->from->y, line->from->z, 
				line->to->x, line->to->y, line->to->z, 
				line->distance);
			addneighbour(line->from, line->to);
			addneighbour(line->to, line->from);
			if (taint(line->from, taintid++) == junctioncount) {
				printf ("%ld x %ld = %ld\n", line->from->x, line->to->x, line->from->x * line->to->x);
				return 0;
			}
			
		}
		line = line->next;
	}

	return 0;
}
