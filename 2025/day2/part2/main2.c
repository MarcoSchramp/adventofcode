
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void strapp(char *s, char *a)
{
	while (*s) s++;
	strcpy(s, a);
}

struct found {
	char *value;
	struct found *next;
};

struct found* top= NULL;

void clearlist()
{
	struct found **f = &top;
	struct found *tmp;

	while (*f) {
		tmp = *f;
		*f = (*f)->next;
		free(tmp->value);
		free(tmp);
	}
}

int addfound(char *s)
{
	struct found **f = &top;
	while (*f) {
		if (strcmp((*f)->value, s) == 0)
			return 0;
		*f = (*f)->next;
	}
	*f = malloc(sizeof **f);
	(*f)->next = NULL;
	(*f)->value = strdup(s);
	return 1;
}

int main(int argc, char* argv[])
{
	char from[100];
	char to[100];
	long sum  = 0;
	while (scanf("%[0123456789]%*[-]%[0123456789]", from , to) != EOF) {
		getchar(); // skip 1 character
		printf("Processing: %s to %s\n", from, to);

		long lfrom = atol(from);
		long lto = atol(to);
		for(int length = 1 ; length <= strlen(from); length++)
		{
			char startpattern[100];
			sprintf(startpattern, "%.*s", length, from);
			if (strlen(from) % length)
				continue;
			int numrepeats = strlen(from) / length;
			if (numrepeats == 1) {
				continue;
			}
			long lhalf = atol(startpattern);
			long ldouble;
			do {
				char d[100];
				*d = 0;
				for(int i = 0; i < numrepeats; i++)
					strapp(d, startpattern);
				ldouble = atol(d);
				printf("Possible found: %s\n", d);
				if (lfrom  <= ldouble && ldouble <= lto) {
					printf("Found: %s\n", d);
					if(addfound(d))
						sum += ldouble;
				}
				lhalf++;
				sprintf(startpattern, "%ld", lhalf);
	
			} while (ldouble <= lto);
		}
		for(int length = 1 ; length <= strlen(to) && strlen(to) != strlen(from); length++)
		{
			if (strlen(to) % length)
				continue;
			int numrepeats = strlen(to) / length;
			if (numrepeats == 1)
				continue;
			char startpattern[100];
			sprintf(startpattern, "%.*s", length, to);
			long lhalf = atol(startpattern);
			long ldouble;
			do {
				char d[100];
				*d = 0;
				for(int i = 0; i < numrepeats; i++)
					strapp(d, startpattern);
				ldouble = atol(d);
				printf("Possible found: %s\n", d);
				if (lfrom  <= ldouble && ldouble <= lto) {
					printf("Found: %s\n", d);
					if(addfound(d))
						sum += ldouble;
				}
				lhalf++;
				sprintf(startpattern, "%ld", lhalf);
	
			} while (ldouble <= lto);
		}
		clearlist();
	}
	printf ("Total = %ld\n", sum);
	return 0;
}
