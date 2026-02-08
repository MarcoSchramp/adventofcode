#include<stdlib.h>
#include<string.h>
#include<stdio.h>

struct charcount {
	char c;
	int count;
};



void setupcharcount(struct charcount* charcount)
{
	for (char c = 'a' ; c <= 'z'; c++) {
		charcount[c - 'a'].c = c;
		charcount[c - 'a'].count = 0;
	}
}

int comparecharcount(const void* left, const void* right)
{
	const struct charcount* l = (const struct charcount*) left;
	const struct charcount* r = (const struct charcount*) right;
	return r->count - l->count;
	
}

int main(int argc, char * argv[])
{
	char line[1024];
	struct charcount charcount[26][26];

	for (int i = 0; i < 30; i++)
		setupcharcount(charcount[i]);

	while (fgets(line, sizeof line, stdin) != NULL) {
		int pos = 0;
		char*l  = line;
		while (*l) {
			if (*l == '\n') l++;
			else charcount[pos++][*(l++) - 'a'].count++;
	
		}
	}
	for (int i = 0; i < 26; i++)
		qsort(charcount[i], sizeof charcount[i] / sizeof *charcount[i], sizeof *charcount[i], comparecharcount);

	for (int i = 0; i < 26; i++) 
		if (charcount[i][0].count > 0)
			printf("%c", charcount[i][0].c);
	printf ("\n");
	
	return 0;
}
