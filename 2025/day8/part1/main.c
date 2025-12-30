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

typedef struct circuit {
	int id;
	int size;
	struct junction* junction;
	struct circuit* next;
} circuit_t;

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

// grootste eerst
int circuit_cmp(circuit_t* left, circuit_t* right)
{
	return left->size > right->size;
}


MERGESORT(line_t, line_cmp)
MERGESORT(circuit_t, circuit_cmp)

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
	long linecount = 0;
	int taintid = 1;
	struct line *line = linelist;
	while (line && (linkedjunctioncount < 1000)) {
		if ((line->from->groupid != line->to->groupid) ||
		   (line->from->groupid == 0 && line->to->groupid == 0)) {
			printf ("%ld: Connecting (%ld,%ld,%ld) to (%ld,%ld,%ld) dist %ld\n",
				linkedjunctioncount,
				line->from->x, line->from->y, line->from->z, 
				line->to->x, line->to->y, line->to->z, 
				line->distance);
			addneighbour(line->from, line->to);
			addneighbour(line->to, line->from);
			taint(line->from, taintid++);
		}
		linkedjunctioncount++;
		line = line->next;
	}
	printf("linecount %ld\n", linecount);	

	for(struct junction *j = junctionlist; j; j = j->next)
		j->groupid = 0;

	circuit_t *circuit_list = NULL;
	taintid = 1;
	for(struct junction *j = junctionlist; j; j = j->next) {
		if (j->groupid == 0) {
			circuit_t* new = malloc(sizeof (*new));
			new->size = taint(j, taintid++);
			new->next = circuit_list;
			circuit_list = new;
			printf ("%d: taintcount %d\n", taintid, new->size);
		}
	}

	circuit_list = circuit_t_sortlist(circuit_list);
	for (circuit_t* c = circuit_list; c; c = c->next) {
		printf ("%d\n", c->size);
	}

	long prod = 1;
	int i = 0;
	circuit_t* c = circuit_list; 
	while (i < 3 && c) {
		prod *= c->size;
		c = c->next;
		i++;
	}

	printf ("product = %ld\n", prod);
		

	return 0;
}
