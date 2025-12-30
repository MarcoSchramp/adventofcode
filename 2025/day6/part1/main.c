
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct problem {
	long value[4];
	char operator;
	long result;
	struct problem *next;
};

struct problem* problemlist;
#define LINELENGTH 10000

int main(int argc, char* argv[])
{
	char *line = malloc(LINELENGTH);
	int maxcol = 0;
	int col = 0;

	struct problem** p = &problemlist;

	// line 1
	fgets(line, LINELENGTH, stdin);
	for(char *tok = strtok(line, " "); tok; tok = strtok(NULL, " ")) {
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
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " ")) {
		(*p)->value[1] =  atol(tok);
		printf ("Line 2 (col = %d): read %ld\n", ++col, (*p)->value[1]);
		p = &(*p)->next;
	}
	// line 3
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH , stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " ")) {
		(*p)->value[2] =  atol(tok);
		printf ("Line 3 (col = %d): read %ld\n", ++col, (*p)->value[2]);
		p = &(*p)->next;
	}
	// line 4
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH , stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " ")) {
		(*p)->value[3] =  atol(tok);
		printf ("Line 4 (col = %d): read %ld\n", ++col, (*p)->value[3]);
		p = &(*p)->next;
	}
	// Line 5
	p = &problemlist;
	col = 0;
	fgets(line, LINELENGTH , stdin);
	for(char *tok = strtok(line, " "); col < maxcol && tok; tok = strtok(NULL, " ")) {
		printf ("Line 5 (col = %d): read %c\n", ++col, *tok);
		(*p)->operator =  *tok;
		p = &(*p)->next;
	}
	printf ("End of reading\n");

	long sum = 0;
	for (p = &problemlist; *p != NULL; p = &(*p)->next)
	{
		printf ("%ld %c %ld %c %ld %c %ld = ", 
			(*p)->value[0],
			(*p)->operator,
			(*p)->value[1],
			(*p)->operator,
			(*p)->value[2],
			(*p)->operator,
			(*p)->value[3]);
		if ((*p)->operator == '+')
			(*p)->result = (*p)->value[0] +
			               (*p)->value[1] +
			               (*p)->value[2] +
			               (*p)->value[3];
		else
			(*p)->result = (*p)->value[0] *
			               (*p)->value[1] *
			               (*p)->value[2] *
			               (*p)->value[3];

		printf ("%ld\n", 
			(*p)->result);
		sum += (*p)->result;
	}
	printf("result: %ld\n", sum);

	return 0;
}
