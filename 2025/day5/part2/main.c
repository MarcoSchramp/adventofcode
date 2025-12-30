#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to hold range
struct range {
	long from;
	long to;
	struct range *next;
} *rangelist = NULL;

// Add range to rangelist
void addrange(long from, long to)
{
	struct range* r = malloc(sizeof (*r));
	r->from = from;
	r->to = to;
	r->next = rangelist;
	rangelist = r;
}


// Merge sort
// Sort list of ranges, algorithm: merge-sort
// returns pointer to first element in sorted list
struct range* sortlist(struct range* r)
{
	// Cannot sort empty list
	if (r == NULL) return r;
	// No need to sort a list of one item
	if (r->next == NULL) return r;


	// Split lists in two equal halfs, both unsorted
	struct range *right = NULL;
	struct range *left = NULL;

	// i is used to switch between left or right list
	int i = 0;
	while (r) {
		struct range *n = r->next;
		if ((i++) % 2) {
			r->next = left;
			left = r;
		}
		else {
			r->next = right;
			right = r;
		}
		r = n;
	}

	// sort each list (recursive)
	left = sortlist(left);
	right = sortlist(right);

	// Start merging;

	// introducing r
	struct range **rtarget = &r;
	while (left || right) {
		// When there's still left & right, then compare
		if (left && right) {
			if (left->from < right->from) {
				*rtarget = left;
				rtarget = &(left->next);
				left = left->next;
			}
			else {
				*rtarget = right;
				rtarget = &(right->next);
				right = right->next;
			}
		}
		// Only left items remaining --> add left
		else if (left) {
			*rtarget = left;
			rtarget = &(left->next);
			left = left->next;
		}
		// Only right items remaining --> add right
		else { // right
			*rtarget = right;
			rtarget = &(right->next);
			right = right->next;
		}
	}
	*rtarget = NULL; // Null terminate the list!
	return r;
}

// If current and next are overlapping ranges, then
// merge into one range
// returns next in case of no merge
// returns current in case of merge (free next)
struct range* mergerange(struct range *current, struct range* next)
{
	if (next == NULL) return NULL;
	if (current->to < next->from)
		return next; // Cannot merge
	if (current->to < next->to)
		current->to = next->to;
	current->next = next->next;
	free(next);
	return current;
}

int main(int argc, char* argv[])
{
	// Line buffer
	char line[1024];


	while (fgets(line, sizeof line, stdin) != NULL) {
		// break on empty line
		if (strcmp(line, "\n") == 0)  break;

		// Parse line
		char *tok = strtok(line, "-");
		long from = atol(tok);
		tok = strtok(NULL, "-");
		long to = atol(tok);

		// add range
		addrange(from, to);
	}

	// sort the range list in ascending order
	rangelist = sortlist(rangelist);

	// Merge range into next when overlapping
	struct range* r = rangelist;
	while (r) 
		r = mergerange(r, r->next);


	// Walk the list and count the number of valid ids
	long idcount = 0;
	for (struct range*r = rangelist; r; r=r->next)
		idcount += r->to - r->from  + 1;
	
	printf ("total ids %ld\n", idcount);
	return 0;
}

