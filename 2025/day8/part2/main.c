#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"marco.h"
// predeclare neighbour
struct neigbour;

// Junction with location in space
struct junction {
	long x;
	long y;
	long z;
	struct junction *next; // Just a linked list for ease of use
	struct neighbour *neighbours; // start of list of connected junctionc
	int groupid;
};

// List of connected junctions
struct neighbour {
	struct junction* junction;
	struct neighbour *next;
};


// Calculated distance between two junctions, to allow for sorting
typedef struct line {
	struct junction* from;
	struct junction* to;
	long distance;
	struct line *next; // List of calculated distances
} line_t;
// Typedef required because MERGESORT macro doesn't like typenames with spaces

// Start of list of junctions
struct junction* junctionlist = NULL;

// Start of list of calculated distances
struct line* linelist = NULL;

// Connect junction to another junction by creating a neighbour in the list
void addneighbour(struct junction* j, struct junction* n)
{
	struct neighbour * new = malloc(sizeof (*new));
	new->next = j->neighbours;
	new->junction = n;
	j->neighbours = new;
}

// Compare function for MERGESORT
// This sorts with shortest distance first
int line_cmp(line_t* left, line_t* right)
{
	return left->distance < right->distance;
}


MERGESORT(line_t, line_cmp)

// Mark group of junctions with same id
// return the number of junctions in the group
int taint(struct junction *j, int id)
{
	if (j->groupid == id) return 0;
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

	// Every elaborate scanf to read coordinates in 1 
	while (scanf("%ld%*[,]%ld%*[,]%ld%*[^\n]", &x, &y, &z) != EOF) {
		
		// Create junction
		*j = malloc(sizeof **j);
		(*j)->x = x;
		(*j)->y = y ;
		(*j)->z = z;
		(*j)->next = NULL;
		(*j)->neighbours = NULL;
		(*j)->groupid = 0;

		// Add to list
		j = &(*j)->next;
		junctioncount++;
	}

	// Calculate distances
	// and build list
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

	// sort distances (shortest first)
	linelist = line_t_sortlist(linelist);


	// Connect junctions (using shortest distance first)
	long linkedjunctioncount = 0;
	int taintid = 1;
	struct line *line = linelist;
	while (line ) {
		// Only connect when not connected at all or when in different sub-circuits
		if ((line->from->groupid != line->to->groupid) || (line->from->groupid == 0 && line->to->groupid == 0)) {

			addneighbour(line->from, line->to);
			addneighbour(line->to, line->from);

			// Stop if the "subcircuit" holds all nodes
			if (taint(line->from, taintid++) == junctioncount) {
				// line is the last line connected
				printf ("%ld x %ld = %ld\n", line->from->x, line->to->x, line->from->x * line->to->x);
				return 0;
			}
			
		}
		line = line->next;
	}

	return 0;
}
