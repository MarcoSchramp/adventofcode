
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// The biggest problem of this exercise is the reading of the input, 
// because it's organized horizontally, instead of vertically

// Each exercise has 4 input values and an operator
struct problem {
	long value[4];
	char operator;
	long result;
	struct problem *next; // we keep a list of problems/exercises
};

struct problem* problemlist; // root of problems/exercises

// line length is very long!
#define LINELENGTH 10000

int main(int argc, char* argv[])
{
	// Because of size, the line cannot be a stack variable anymore
	char *line = malloc(LINELENGTH);
	int maxcol = 0;
	int col = 0;

	struct problem** p = &problemlist;

	// Lazy approach: almost repeated code for each numbers line :)

	// line 1
	fgets(line, LINELENGTH, stdin);
	for(char *tok = strtok(line, " "); tok; tok = strtok(NULL, " ")) {
		// Allocate memory for new problem (this is not needed anymore for the next lines)
		*p = malloc (sizeof **p);
		(*p)->value[0] =  atol(tok);
		(*p)->next = NULL;
		++maxcol;
		p = &(*p)->next;
	}

	// line 2
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH, stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " "), col++) {
		(*p)->value[1] =  atol(tok);
		p = &(*p)->next;
	}
	// line 3
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH , stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " "), col++) {
		(*p)->value[2] =  atol(tok);
		p = &(*p)->next;
	}
	// line 4
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH , stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " "), col++) {
		(*p)->value[3] =  atol(tok);
		p = &(*p)->next;
	}
	// Line 5
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH , stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " "), col++) {
		(*p)->operator =  *tok;
		p = &(*p)->next;
	}

	// All problems/exercises loaded, now start math!

	long sum = 0;
	for (p = &problemlist; *p != NULL; p = &(*p)->next)
	{
		if ((*p)->operator == '+')
			(*p)->result = (*p)->value[0] + (*p)->value[1] + (*p)->value[2] + (*p)->value[3];
		else
			(*p)->result = (*p)->value[0] * (*p)->value[1] * (*p)->value[2] * (*p)->value[3];

		sum += (*p)->result;
	}
	printf("result: %ld\n", sum);

	return 0;
}
