
#include<stdlib.h>
#include<stdio.h>
#include<string.h>


// This problem started at the wrong tangent: the text suggested
// a packacking optimization problem needed to be solved
// My head was breaking, because this is known to be an NP-hard
// problem
//
// The final solution is therefore much simpler:
// - full fit: no dense packing needed (
// - possible fit: it estimates the space needed (25% more than densest packing)
//   must be less than total surface

#define LINELENGTH 100

// Packages are always fit on a 3x3
#define PACKAGEWIDTH 3
#define PACKAGEHEIGHT 3

// Definition cannot contain more than 10 packages
#define MAXPACKAGE 10

// Strucure to hold package map
struct package {
	char pattern[PACKAGEWIDTH][PACKAGEHEIGHT];
};

struct package packages[MAXPACKAGE];

void setpackageline (struct package* p, char *line, int lineno)
{
	for(int i = 0; i < PACKAGEWIDTH; i++)
		p->pattern[lineno][i] = line[i] == '#';
}

int countpackage(struct package* p)
{
	int count = 0;
	for(int row = 0; row < PACKAGEHEIGHT; row++)
		for (int col = 0; col < PACKAGEWIDTH; col++)
			count += p->pattern[row][col];
	return count;
}

int fitsheet(int width, int height, int* packagecount, int packagecountsize)
{
	int sum = 0;
	int sumspaces = 0;

	// Count the total number of packages
	// Count the number of positions required in case of dense packing
	for (int i = 0; i < packagecountsize; i++) {
		sum += packagecount[i];
		int c = countpackage(packages+i);
		sumspaces += c * packagecount[i];
	}

	// Simple rectangle
	int surface = width * height;

	// Test if non-dense packing fits
	if (sum * PACKAGEWIDTH * PACKAGEWIDTH  < surface) {
		return 1;
	}

	// Test if there is 25% overhead room to handle semi-dense packing
	if (sumspaces * 1.25 < surface) {
		return 1;
	}


	// Not enough overhead room --> doesn't fit
	return 0;
}


	
int main (int argc, char* argv[])
{
	// Line buffer
	char line[LINELENGTH];

	// Contains the number of packages to pack of each type
	int count[MAXPACKAGE];

	// Clear the packages storage
	memset(packages, 0, sizeof packages);


	// Count the number of configurations that fit
	int fitcount = 0;

	// During parsing: current package reading, after reading package formats : the upper limit of packages
	int packageno = 0;
	// Line number when parsing package structure
	int packagelineno = 0;
	while (fgets(line, sizeof line, stdin) != NULL) {

		if (strchr(line, 'x')) {
			// Lines with an "x" must be a surface with package list
			// parse a sheet line ###x### : ### #### ### ### ###  ####
			// Use x, :, <space> and <nl> as separator
			char * widthtext = strtok(line, "x: \n");
			char * heighttext = strtok(NULL, "x: \n");
			int width = atoi(widthtext);
			int height = atoi(heighttext);

			// Set the number of packages on a surface
			int p = 0;
			for (char* t = strtok(NULL, "x: \n"); t; t = strtok(NULL, "x: \n")) 
				count[p++] = atoi(t);

			// Validate fit
			fitcount += fitsheet(width, height, count, p);

		}
		else if (strchr(line,':')) {
			// parse a package number line
			// Ignore package number
		}
		else if (!strcmp(line,"\n")) {
			// parse separator line --> terminator of package
			packageno++;
			packagelineno = 0;
		}
		else {
			// parse pattern
			setpackageline(packages+packageno, line, packagelineno++);
		}
	}
	printf ("Fit count = %d\n", fitcount);
	return 0;
}
