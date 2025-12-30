#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct range {
	long from;
	long to;
	struct range *next;
} *rangelist = NULL;

void addrange(long from, long to)
{
	struct range* r = malloc(sizeof (*r));
	r->from = from;
	r->to = to;
	r->next = rangelist;
	rangelist = r;
}

struct range* inrange(long id)
{
	struct range *r = rangelist;
	while (r) {
		if (r->from <= id && id <= r->to)
			return r;
		r = r->next;
	}
	return NULL;
}

void printlist(struct range *r, char *s) 
{
	printf ("------ %s ----\n", s);
	for (; r; r = r->next)
		printf ("%15ld - %15ld\n", r->from , r->to);
}

// Merge sort
struct range* sortlist(struct range* r)
{
	if (r == NULL) return r;
	if (r->next == NULL) return r;
	int i = 0;
	struct range *right = NULL;
	struct range *left = NULL;

	//split
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

	// sort each list
	left = sortlist(left);
	right = sortlist(right);

	// Start merging;
	printlist(left, "Left");
	printlist(right, "Right");
	struct range **rtarget = &r;
	while (left || right) {
		if (left && right) {
			if (left->from < right->from) {
				printf("Adding %ld\n", left->from);
				*rtarget = left;
				rtarget = &(left->next);
				left = left->next;
			}
			else {
				printf("Adding %ld\n", right->from);
				*rtarget = right;
				rtarget = &(right->next);
				right = right->next;
			}
		}
		else if (left) {
			printf("Adding %ld\n", left->from);
			*rtarget = left;
			rtarget = &(left->next);
			left = left->next;
		}
		else { // right
			printf("Adding %ld\n", right->from);
			*rtarget = right;
			rtarget = &(right->next);
			right = right->next;
		}
	}
	*rtarget = NULL; // Null terminate the list!
	printlist(r, "Merged");
	return r;
}

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
	rangelist = sortlist(rangelist);
	printlist(rangelist, "final");

	struct range* r = rangelist;
	while (r) 
		r = mergerange(r, r->next);

	printlist(rangelist, "reduced");

	long idcount = 0;
	for (struct range*r = rangelist; r; r=r->next)
		idcount += r->to - r->from  + 1;
	
	printf ("total ids %ld\n", idcount);
	return 0;
}

