#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	unsigned int keys[100]; // max 100 keys;
	char line[1000];
	int total = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		int target = 0;
		int keycount = 0;

		char *strtok_space = NULL;
		for (char *part = strtok_r(line, " ", &strtok_space);
		     part ; part = strtok_r(NULL, " ", &strtok_space)) {
			if (*part == '[') {
				char *light = line + 1;
				int pos = 0;
				while (*light != ']') {
					if (*(light++) == '#')
						target |= 1 << pos;
					pos ++;
				}
			}
			if (*part == '(') {
				char *strtok_idx = NULL;
				unsigned int key = 0;
				for(char *idx = strtok_r(part + 1, ",)", &strtok_idx);
				    idx; idx = strtok_r(NULL, ",)", &strtok_idx)) {
					int index = atoi(idx);
					key |= 1 << index;
				}
				keys[keycount++] = key;
				printf ("key %o\n", key);
			}
		}
		printf ("target %o\n", target);
		printf ("keycount %d\n", keycount);
		int minkeystroke = 10000;
		for (unsigned int i = 0; i < 1 << keycount; i++) {
			unsigned int bits = 0;
			int bitcount = 0;
			int keynr = 0;
			int tmp = i;
			while(tmp) {
				if (tmp & 1) {
					bits ^= keys[keynr];
					bitcount++;
				}
				keynr++;
				tmp >>= 1;
			}
			if (bits == target) {
				printf("Found sequence %o with %d keystrokes\n", i, bitcount);
				if (bitcount < minkeystroke)
					minkeystroke = bitcount;
			}
			else 
				printf("Sequence %o: no match \n", i);

		}
		printf ("\n");
		total += minkeystroke;
		
	}
	printf("Total: %d\n", total);

	
	return 0;
}

