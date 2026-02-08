#include<stdlib.h>
#include<string.h>
#include<stdio.h>

struct charcount {
	char c;
	int count; }
charcount[26];

void setupcharcount()
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
	if (l->count != r->count)
		return r->count - l->count;
	return l->c - r->c;
	
}

int main(int argc, char * argv[])
{
	char line[1024];
	char *linefragments[20];
	while (fgets(line, sizeof line, stdin) != NULL) {
		setupcharcount();
		int fragcount  = 0;
		for(char *fragment = strtok(line, "-[]\n"); fragment ; fragment = strtok(NULL, "-[]\n")) {
			if (*fragment >= 'a' && *fragment <= 'z') {
				linefragments[fragcount++] =  fragment;
				while (*fragment) 
					charcount[*(fragment++) - 'a'].count++;
			}
			else {
				long sectorID = atol(fragment);

				qsort(charcount, sizeof charcount / sizeof *charcount, sizeof *charcount, comparecharcount);

				fragment =  strtok(NULL, "-[]\n");
				int isreal = 1;
				for(int i = 0; i < 5; i++) {
					isreal = isreal && fragment[i] == charcount[i].c;
				}
				if (isreal) {
					for (int i = 0; i < fragcount; i++) {
						for (char *p = linefragments[i]; *p; p++)
							*p = 'a' + (*p - 'a' + sectorID) % 26;
						printf ("%s ",linefragments[i]);
				
					}
					printf (": %ld\n", sectorID);
				}
			}
		}
	}
	
	return 0;
}
