#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Valid range of ingredient ids
struct range {
	long from;
	long to;
	struct range *next; // use a linked list
} *rangelist = NULL; // and here is the root of the ll

// Add range to list
void addrange(long from, long to)
{
	struct range* r = malloc(sizeof (*r));
	r->from = from;
	r->to = to;
	r->next = rangelist;
	rangelist = r;
}

// find range that id belongs to
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

int main(int argc, char* argv[])
{
	// read line buffer
	char line[1024];
	int count = 0;

	// First part: read ranges
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

	// Send part: validate ids
	while (fgets(line, sizeof line, stdin) != NULL) {
		long id = atol(line);
		struct range* r = inrange(id);
		if (r) 
			count++;
	}
	printf ("Count = %d\n", count);
	return 0;
}

