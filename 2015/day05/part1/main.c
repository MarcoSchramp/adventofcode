#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	// read buffer
	char line[1024];
	int nicecount = 0;
	

	while (scanf("%s", line) != EOF) // Scanf nicely strips the \n
	{
		int aeioucount = 0;
		int doublefound = 0;
		int illegalsequencefound = 0;
		

		for (char *s = line; *s; s++) {
			if (strchr("aeiou", *s)) aeioucount++;
			if (*s && *s == *(s+1))
				doublefound++;
		}

		illegalsequencefound |= strstr(line, "ab") != NULL;
		illegalsequencefound |= strstr(line, "cd") != NULL;
		illegalsequencefound |= strstr(line, "pq") != NULL;
		illegalsequencefound |= strstr(line, "xy") != NULL;

		int nice = aeioucount >= 3 && doublefound && !illegalsequencefound;
		nicecount += nice;
	}
	printf("Nice count = %d\n", nicecount);
}

