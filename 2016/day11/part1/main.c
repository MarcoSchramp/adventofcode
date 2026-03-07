#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

#undef TEST
#define PART2

#ifdef TEST

#define NR_OF_ELEMENTS 2
#define ELEMENTS_MAX_BIT 0x4
#define ELEMENTS_MASK 0x03
#define MAXFLOORS 4

#define HYDROGEN     1
#define LITHIUM      2

#else

#ifdef PART2

#define NR_OF_ELEMENTS 7
#define ELEMENTS_MAX_BIT 0x80
#define ELEMENTS_MASK 0x7F

#else

#define NR_OF_ELEMENTS 5
#define ELEMENTS_MAX_BIT 0x20
#define ELEMENTS_MASK 0x1F

#endif

#define MAXFLOORS 4

#define PROMETHEUM     1
#define COBALT         2
#define CURIUM         4
#define RUTHENUIM      8
#define PLUTONIUM      16
#define ELERIUM        32
#define DILITHIUM      64

#endif

int issafe(unsigned int chip_state, unsigned int generator_state)
{
	// Loop over all bits
	for (unsigned int i = 1; i < ELEMENTS_MAX_BIT; i = i << 1)
	{
		if ((chip_state & i) && (generator_state & i) == 0 && (generator_state & (~i)))
			return 0; // chip not attached to generator, other generator in room --> fail
	}
	return 1;
}

int issafematrix[ELEMENTS_MAX_BIT][ELEMENTS_MAX_BIT];

void initsafematrix() {
	for (unsigned int chip_state = 0; chip_state < ELEMENTS_MAX_BIT; chip_state++) {
		for (unsigned int generator_state = 0; generator_state < ELEMENTS_MAX_BIT; generator_state++) {
			int is = issafe(chip_state, generator_state);
			issafematrix[chip_state][generator_state] = is;
		}
	}
}

struct floorstate {
	int chip_state;
	int generator_state;
};


struct systemstate {
	struct floorstate floorstate[MAXFLOORS];
	int floor;
};


int istargetstate(struct systemstate *ss)
{
	for (int i = 0; i < MAXFLOORS - 1; i++)
		if (ss->floorstate[i].chip_state || ss->floorstate[i].generator_state)
			return 0;
	return 1;
}

struct takealong {
	struct takealong* next;
	struct floorstate floorstate;
};

struct takealong *takealonglist = NULL;
void initpossibletakealongs()
{
	for (unsigned int i = 1; i < ELEMENTS_MAX_BIT * ELEMENTS_MAX_BIT; i = i << 1) {
		for (unsigned int j = i; j < ELEMENTS_MAX_BIT * ELEMENTS_MAX_BIT; j = j << 1) {
			unsigned int together = i | j;
			unsigned int chip_state = together & ELEMENTS_MASK;
			unsigned int generator_state = (together >>  NR_OF_ELEMENTS) & ELEMENTS_MASK;

			// Can only put safe combinations in the elevator
//			if (issafematrix[chip_state][generator_state]) {
				struct takealong* takealong = malloc(sizeof *takealong);
				takealong->next = takealonglist;
				takealong->floorstate.chip_state = chip_state;
				takealong->floorstate.generator_state = generator_state;
				takealonglist = takealong;
//			}
		}
	}
}

			
struct systemstatelist {
	struct systemstatelist* source;
	struct systemstatelist* next;
	struct systemstate systemstate;
	int stepno;
};

struct BTreeNode* btreeroot = NULL;


struct systemstatelist* lastsystemstatelist = NULL;
// List of reached states so far
// pointer in list of systemstatelist. Elements before this pointer have been evaluated as potential, elements after this list have not.
struct systemstatelist* potentiallist = NULL;

BTreeKeyType buildbtreekey(struct systemstate* systemstate) 
{

	BTreeKeyType btreekey = 0;
	btreekey = ((uint64_t) systemstate->floor) << 60;
	for (uint64_t floor = 0; floor < MAXFLOORS; floor++) {
		int mask  = 1;
		for(int i = 0; i < NR_OF_ELEMENTS; i++, mask = mask << 1) {
			if (systemstate->floorstate[floor].chip_state & mask)
				btreekey = btreekey | (floor << (i * 4));	
			if (systemstate->floorstate[floor].generator_state & mask)
				btreekey = btreekey | (floor << (i * 4 + 32));	
		}
	}
	return btreekey;
}

void newsystemstatelist(struct systemstate* systemstate, int stepno, struct systemstatelist* source)
{
	struct systemstatelist* new = malloc (sizeof *new);
	new->source = source;
	new->next = NULL;
	new->systemstate = *systemstate;
	new->stepno = stepno;
	if (lastsystemstatelist)
		lastsystemstatelist->next = new;
	lastsystemstatelist = new;
	if (!potentiallist)
		potentiallist = new;

}



struct systemstatelist* takesystemstatelist()
{
	struct systemstatelist* result = potentiallist;
	if (potentiallist)
		potentiallist = potentiallist->next;
	return result;
}

void printbackstrack(struct systemstatelist* ssl)
{
	if (ssl-> source)
		printbackstrack(ssl->source);
	uint64_t key  = buildbtreekey(&ssl->systemstate);
	printf(" --> %16.16lx\n", key);
}

int laststepno = -1;

int processpotentials()
{
	struct systemstatelist *potential = takesystemstatelist();
	if (!potential)
		return -1;
	struct BTreeNode* node = findBTreeNode(btreeroot, buildbtreekey(&potential->systemstate));
	if (node)
		return 0;
	if (potential->stepno != laststepno) {
		laststepno = potential->stepno;
		printf("Step %d\n", laststepno);
	}
	if (istargetstate(&potential->systemstate)) {
		printf ("Minimum steps found --> %d\n", potential->stepno);
	}

	struct systemstate* systemstate = &potential->systemstate;

	int floor = systemstate->floor;
	for (struct takealong *takealong = takealonglist; takealong; takealong = takealong->next) {
		if (((systemstate->floorstate[floor].chip_state & takealong->floorstate.chip_state) == takealong->floorstate.chip_state) &&
		    ((systemstate->floorstate[floor].generator_state & takealong->floorstate.generator_state) == takealong->floorstate.generator_state)) {
			// All ingredients are on this floor

			if (floor > 0) {
				// We can go down
				struct systemstate targetsystemstate = *systemstate;
				// remove items from thos floor
				targetsystemstate.floorstate[floor].chip_state = targetsystemstate.floorstate[floor].chip_state & ~ takealong->floorstate.chip_state;
				targetsystemstate.floorstate[floor].generator_state = targetsystemstate.floorstate[floor].generator_state & ~ takealong->floorstate.generator_state;

				// put them on the flow below
				targetsystemstate.floorstate[floor-1].chip_state = targetsystemstate.floorstate[floor-1].chip_state | takealong->floorstate.chip_state;
				targetsystemstate.floorstate[floor-1].generator_state = targetsystemstate.floorstate[floor-1].generator_state | takealong->floorstate.generator_state;
				targetsystemstate.floor--;

				// Check if safe on destination
				if (issafematrix[targetsystemstate.floorstate[floor].chip_state][targetsystemstate.floorstate[floor].generator_state] &&
				    issafematrix[targetsystemstate.floorstate[floor-1].chip_state][targetsystemstate.floorstate[floor-1].generator_state]) {
					// Check if this end-state has been evaluated before or is ready to be evaluated

					newsystemstatelist(&targetsystemstate, potential->stepno + 1, potential);
				}
			}
			if (floor < MAXFLOORS - 1) {

				// We can go up
				struct systemstate targetsystemstate = *systemstate;

				// remove items from this flor
				targetsystemstate.floorstate[floor].chip_state = targetsystemstate.floorstate[floor].chip_state & ~ takealong->floorstate.chip_state;
				targetsystemstate.floorstate[floor].generator_state = targetsystemstate.floorstate[floor].generator_state & ~ takealong->floorstate.generator_state;

				// put them on the floor above
				targetsystemstate.floorstate[floor+1].chip_state = targetsystemstate.floorstate[floor+1].chip_state | takealong->floorstate.chip_state;
				targetsystemstate.floorstate[floor+1].generator_state = targetsystemstate.floorstate[floor+1].generator_state | takealong->floorstate.generator_state;
				targetsystemstate.floor++;

				// Check if safe on destination
				if (issafematrix[targetsystemstate.floorstate[floor].chip_state][targetsystemstate.floorstate[floor].generator_state] &&
				    issafematrix[targetsystemstate.floorstate[floor+1].chip_state][targetsystemstate.floorstate[floor+1].generator_state]) {
					newsystemstatelist(&targetsystemstate, potential->stepno + 1, potential);
				}
			}
		}
			
	}
	insert(&btreeroot, buildbtreekey(&potential->systemstate));
	return 0; // Not found, continue processing
	
}

int main(int argc, char* argv[])
{
	printf("Start\n");
	initsafematrix();
	printf("Safe matrix initialized\n");
	initpossibletakealongs();
	printf("Take alongs initialized\n");
	struct systemstate initialsystemstate;

#ifdef TEST
	initialsystemstate.floorstate[0].chip_state = LITHIUM | HYDROGEN;
	initialsystemstate.floorstate[0].generator_state = 0;

	initialsystemstate.floorstate[1].chip_state = 0; // Empty
	initialsystemstate.floorstate[1].generator_state = HYDROGEN;

	initialsystemstate.floorstate[2].chip_state =  0;
	initialsystemstate.floorstate[2].generator_state = LITHIUM; // Empty

	initialsystemstate.floorstate[3].chip_state = 0; // Empty
	initialsystemstate.floorstate[3].generator_state = 0; // Empty
#else

#ifdef PART2
	initialsystemstate.floorstate[0].chip_state = PROMETHEUM | ELERIUM | DILITHIUM;;
	initialsystemstate.floorstate[0].generator_state = PROMETHEUM | ELERIUM | DILITHIUM;;

	initialsystemstate.floorstate[1].chip_state = 0; // Empty
	initialsystemstate.floorstate[1].generator_state = COBALT | CURIUM | RUTHENUIM | PLUTONIUM;

	initialsystemstate.floorstate[2].chip_state =  COBALT | CURIUM | RUTHENUIM | PLUTONIUM;
	initialsystemstate.floorstate[2].generator_state = 0; // Empty

	initialsystemstate.floorstate[3].chip_state = 0; // Empty
	initialsystemstate.floorstate[3].generator_state = 0; // Empty

#else
	initialsystemstate.floorstate[0].chip_state = PROMETHEUM;
	initialsystemstate.floorstate[0].generator_state = PROMETHEUM;

	initialsystemstate.floorstate[1].chip_state = 0; // Empty
	initialsystemstate.floorstate[1].generator_state = COBALT | CURIUM | RUTHENUIM | PLUTONIUM;

	initialsystemstate.floorstate[2].chip_state =  COBALT | CURIUM | RUTHENUIM | PLUTONIUM;
	initialsystemstate.floorstate[2].generator_state = 0; // Empty

	initialsystemstate.floorstate[3].chip_state = 0; // Empty
	initialsystemstate.floorstate[3].generator_state = 0; // Empty
#endif
#endif
	initialsystemstate.floor = 0; // On the ground floor


	newsystemstatelist(&initialsystemstate, 0, NULL);

	printf("Start processing\n");
	while (!processpotentials()) {
	}

	return 0;
}
