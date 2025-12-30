#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	char line[1024];

	int arrowpos =  50;
	int zerocount = 0;

	while (fgets(line, sizeof(line), stdin) != NULL) {
		printf("%s", line);
		char cmd = line[0];
		int count = atoi(line+1);
		while (count >= 100) {
			zerocount++;
			count -= 100;
		}
		if (cmd == 'L') {
			if (arrowpos > 0 && arrowpos - count <= 0) {
				zerocount++;
			}
			arrowpos = (arrowpos - count + 100) % 100;
		}
		else if (cmd == 'R') {
			if (arrowpos + count >= 100) {
				zerocount++;
			}
			arrowpos = (arrowpos + count) % 100;
		}
	}

	printf("Password = %d\n", zerocount);
	return 0;
}

