#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"marco.h"

// predeclare structure neigbour
struct neigbour;


// Structure represeting a junction in the network
struct junction {
	// Line number in file
	// position in space
	long x;
	long y;
	long z;

	// A list of junctions (just to keep them accessible)
	struct junction *next;

	// Root of list, connecting the neighbours
	struct neighbour *neighbours;

	// Identifier to distinguish sub-nets
	int groupid;
};

// List of neigbours, each refering to other connected junctions
struct neighbour {
	struct junction* junction;
	struct neighbour *next;
};


// Informaton of distance between two junctions, has square distance between junctions
typedef struct line {
	struct junction* from;
	struct junction* to;
	long distance;
	struct line *next;
} line_t;
// The typedef name is required, because the MERGESORT macros don't work well
// with typenames that contain spaces


// structure to collect information about a circuit and the entry to one 
// of the junctions
typedef struct circuit {
	int id;
	int size;
	struct junction* junction;
	struct circuit* next; // List of circuits
} circuit_t;
// The typedef name is required, because the MERGESORT macros don't work well
// with typenames that contain spaces

// Junction list
struct junction* junctionlist = NULL;

// List of possible distances
struct line* linelist = NULL;

// Add neighbour to neighbourlist of j
void addneighbour(struct junction* j, struct junction* n)
{
	struct neighbour * new = malloc(sizeof (*new));
	new->next = j->neighbours;
	new->junction = n;
	j->neighbours = new;
}

// Used for sorting, returns 1 if left has shorter distance than right
// This allows for sorting small to large
int line_cmp(line_t* left, line_t* right)
{
	return left->distance < right->distance;
}

// grootste eerst
// User for sorting, returns 1 if left has more elements in circuit 
// than right
// This allows for sorting circuits from large to small
int circuit_cmp(circuit_t* left, circuit_t* right)
{
	return left->size > right->size;
}

// You've got to love C++, with it's templates, 
// but this also works :)

MERGESORT(line_t, line_cmp)
MERGESORT(circuit_t, circuit_cmp)

// count junctions by setting groupid
// Also set groupid on all neighbours by calling taint
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
	struct junction ** j =  &junctionlist;

	// Very elaborate scanf to read x, y and z in one statement :)
	while (scanf("%ld%*[,]%ld%*[,]%ld%*[^\n]", &x, &y, &z) != EOF) {
		// create junction
		*j = malloc(sizeof **j);
		(*j)->x = x;
		(*j)->y = y ;
		(*j)->z = z;
		(*j)->next = NULL;
		(*j)->neighbours = NULL;
		(*j)->groupid = 0;

		// Append at end of list
		j = &(*j)->next;
	}

	// Calculate a list of lines, where each line refers
	// to both junctions and has distance
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

	// Sort the list of lines from shortest to longest
	linelist = line_t_sortlist(linelist);

	long linkedjunctioncount = 0;
	long linecount = 0;
	int taintid = 1;
	struct line *line = linelist;

	// We only need the first 1000 connectors to be connected 
	while (line && (linkedjunctioncount < 1000)) {
		// Only the junction is alone or not from the same subnet (groupid)
		if ((line->from->groupid != line->to->groupid) || (line->from->groupid == 0 && line->to->groupid == 0)) {

			// Connect the two junctions (make them neighbour)
			addneighbour(line->from, line->to);
			addneighbour(line->to, line->from);

			// reset the (merged) subnets, give them a new group-id
			taint(line->from, taintid++);
		}

		// Go to next possible connector
		linkedjunctioncount++;
		line = line->next;
	}

	// reset group-ids
	for(struct junction *j = junctionlist; j; j = j->next)
		j->groupid = 0;


	// re-taint all junctions to determine their sizes
	// And put these in the ciruit list
	circuit_t *circuit_list = NULL;
	taintid = 1;
	for(struct junction *j = junctionlist; j; j = j->next) {
		if (j->groupid == 0) {
			circuit_t* new = malloc(sizeof (*new));
			new->size = taint(j, taintid++);
			new->next = circuit_list;
			circuit_list = new;
		}
	}

	// Sort the circuit list (largest to smallest)
	circuit_list = circuit_t_sortlist(circuit_list);

	// Multiply the sizes of the 3 largest circuits
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
