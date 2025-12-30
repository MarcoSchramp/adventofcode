
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define LINELENGTH 100
#define PACKAGEWIDTH 3
#define PACKAGEHEIGHT 3
#define MAXPACKAGE 10

struct package {
	struct package* next;
	char pattern[PACKAGEWIDTH][PACKAGEHEIGHT];
};

struct package *packages[MAXPACKAGE];

void addpackage(int packageno)
{
	struct package *p = malloc(sizeof *p);
	memset(p->pattern, 0, sizeof p->pattern);
	p->next = packages[packageno];
	packages[packageno] = p;
}

void printpackage(struct package *p) 
{
	for(int row = 0; row < PACKAGEHEIGHT; row++) {
		for (int col = 0; col < PACKAGEWIDTH; col++)
			printf ("%s", p->pattern[row][col] ? "#" : ".");
		printf("\n");
	}
}

void setpackageline (struct package* p, char *line, int lineno)
{
	for(int i = 0; i < PACKAGEWIDTH; i++)
		p->pattern[i][lineno] = line[i] == '#';
}

int countpackage(struct package* p)
{
	int count = 0;
	for(int row = 0; row < PACKAGEHEIGHT; row++)
		for (int col = 0; col < PACKAGEWIDTH; col++)
			count += p->pattern[row][col] != 0;
	return count;
}

struct package* mirror(struct package* p)
{
	struct package *p_new = malloc(sizeof *p);
	p_new->next = NULL;
	for(int row = 0; row < PACKAGEWIDTH; row++)
		for(int col = 0; col < PACKAGEWIDTH; col++)
			p_new->pattern[row][col] = p->pattern[PACKAGEWIDTH-row - 1][col];
	return p_new;
			
}
struct package* rotate(struct package* p)
{
	struct package *p_new = malloc(sizeof *p);
	p_new->next = NULL;
	for(int row = 0; row < PACKAGEWIDTH; row++)
		for(int col = 0; col < PACKAGEWIDTH; col++)
			p_new->pattern[row][col] = p->pattern[PACKAGEWIDTH - col - 1][row];
	return p_new;
			
}

int match(struct package* p_list, struct package* p) {
	while (p_list) {
		int m = 1;
		for(int row = 0; row < PACKAGEWIDTH; row++)
			for(int col = 0; col < PACKAGEWIDTH; col++)
				m &= p_list->pattern[row][col] == p->pattern[row][col];
		if (m)
			return 1;
		p_list = p_list->next;
	}
	return 0;
}

void expandpackage(struct package* p_list)
{
	//printpackage(p_list);
	struct package* rot_p = p_list;
	for(int i  = 0; i < 4; i++) {
		rot_p = rotate(rot_p);
		if (!match(p_list, rot_p)) {
			rot_p->next=  p_list->next;
			p_list->next = rot_p;
		}
		// else printf("Match --> Ignore\n");
	}

	rot_p = mirror(p_list);
	for(int i  = 0; i < 4; i++) {
		rot_p = rotate(rot_p);
		if (!match(p_list, rot_p)) {
			rot_p->next=  p_list->next;
			p_list->next = rot_p;
		}
		// else printf("Match --> Ignore\n");
	}
}

int fitsheet(int width, int height, int* packagecount, int packagecountsize)
{
	int sum = 0;
	int sumspaces = 0;
	for (int i = 0; i < packagecountsize; i++) {
		sum += packagecount[i];
		sumspaces += countpackage(packages[i]) * packagecount[i];
	}

	int surface = width * height;

	printf("Surface required %4d, max required %4d available surface %4d, remaining surface %4d\n", sumspaces, sum * PACKAGEWIDTH * PACKAGEWIDTH, surface, surface - sumspaces);

	if (sum * PACKAGEWIDTH * PACKAGEWIDTH  < surface) {
		// Will will in all patterns, just exit
		printf("Fit (Full)\n");
		return 1;
	}
	if (sum * PACKAGEWIDTH * PACKAGEWIDTH < width * height * 1.25) {
		printf("Fit (partial)\n");
		return 1;
	}
	
	printf("No fit!\n");


	return 0;
}


	
int main (int argc, char* argv[])
{
	char line[LINELENGTH];
	int count[MAXPACKAGE];
	memset(packages, 0, sizeof packages);

	int packageno = 0;
	int packagelineno = 0;
	int fitcount = 0;
	while (fgets(line, sizeof line, stdin) != NULL) {
		printf("%s", line);
		if (strchr(line, 'x')) {
			// parse a sheet line
			char * widthtext = strtok(line, "x: \n");
			char * heighttext = strtok(NULL, "x: \n");
			int width = atoi(widthtext);
			int height = atoi(heighttext);
			int p = 0;
			for (char* t = strtok(NULL, "x: \n"); t; t = strtok(NULL, "x: \n")) 
				count[p++] = atoi(t);
			fitcount += fitsheet(width, height, count, p);

		}
		else if (strchr(line,':')) {
			// parse a package number line
			addpackage(packageno);
		}
		else if (!strcmp(line,"\n")) {
			// parse separator line
			expandpackage(packages[packageno]);
			packageno++;
			packagelineno = 0;
		}
		else {
			// parse pattern
			setpackageline(packages[packageno], line, packagelineno++);
		}
	}
	printf ("Fit count = %d\n", fitcount);
	return 0;
}
