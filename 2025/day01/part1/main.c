#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	char line[1024];

	int arrowpos =  50;
	int zerocount = 0;

	while (fgets(line, sizeof(line), stdin) != NULL) {
		char cmd = line[0];
		int count = atoi(line+1);
		if (cmd == 'L')
			arrowpos = (arrowpos - count) % 100;
		else if (cmd == 'R')
			arrowpos = (arrowpos + count) % 100;
		if (arrowpos == 0)
			zerocount++;
	}

	printf("Password = %d\n", zerocount);
	return 0;
}

