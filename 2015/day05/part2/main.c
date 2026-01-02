#include <stdio.h>

int main(int argc, char* argv[])
{
	// read buffer
	char line[1024];
	int nicecount = 0;
	

	while (scanf("%s", line) != EOF) // Scanf nicely strips the \n
	{
		int repeatfound = 0;
		int doublefound = 0;


		for (char *s = line; *s; s++) {
			if (*s && *(s+1)) {
				for (char* fnd = s + 2; *fnd && *(fnd+1); fnd++)
					repeatfound |= *s == *fnd && *(s+1) == *(fnd+1);
			}
			if (*s && *(s+1) && *s == *(s+2) && *s != *(s+1))
				doublefound++;
		}


		int nice = repeatfound && doublefound;
		nicecount += nice;
	}
	printf("Nice count = %d\n", nicecount);
}

