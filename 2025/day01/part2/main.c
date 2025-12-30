#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	// Read linebuffer
	char line[1024];

	// Initial dial position = 50
	int arrowpos =  50;
	int zerocount = 0;

	while (fgets(line, sizeof(line), stdin) != NULL) {
		// Get command (left / right)
		char cmd = line[0];
		// Get nr of clicks
		int count = atoi(line+1);

		// Deal with dial going around several times
		// direction is irrelevant here
		while (count >= 100) {
			zerocount++;
			count -= 100;
		}

		if (cmd == 'L') {
			// Check if arrow goes through 0
			if (arrowpos > 0 && arrowpos - count <= 0) {
				zerocount++;
			}

			// (additional 100 to account for negative values of count
			arrowpos = (arrowpos - count + 100) % 100;
		}
		else if (cmd == 'R') {
			// Check if arrow goes through 100 (aka 0)
			if (arrowpos + count >= 100) {
				zerocount++;
			}
			arrowpos = (arrowpos + count) % 100;
		}
	}

	printf("Password = %d\n", zerocount);
	return 0;
}

