#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	// Line buffer (assuming this fits!)
	char line[1000];

	// Assuming no more than
	unsigned int keys[100]; // max 100 keys;
	int total = 0;

	// Get a line
	while (fgets(line, sizeof line, stdin) != NULL) {
		int target = 0;
		int keycount = 0;

		// Parse the line using strtok_r
		char *strtok_space = NULL;

		// The tree parts are separated by spaces
		for (char *part = strtok_r(line, " ", &strtok_space);
		     part ; part = strtok_r(NULL, " ", &strtok_space)) {
			//The first part with target lights starts with 
			if (*part == '[') {
				char *light = line + 1;
				int pos = 0;
				//target lights ends with ]
				while (*light != ']') {
					// Create bitmask, set bits
					// for each light that is on
					if (*(light++) == '#')
						target |= 1 << pos;
					pos++;
				}
			}
			// The parts with the button definitions starts with (
			if (*part == '(') {
				char *strtok_idx = NULL;
				unsigned int key = 0;
				// Split on comma and termination
				for(char *idx = strtok_r(part + 1, ",)", &strtok_idx);
				    idx; idx = strtok_r(NULL, ",)", &strtok_idx)) {
					// Create bitmask for button
					int index = atoi(idx);
					key |= 1 << index;
				}
				keys[keycount++] = key;
			}
			// ignore the "{" sections
		}

		// Assume any valid combination will be less than 10000 strokes
		// Since we know it cannot be more than the number of buttons
		// this is a valid assumption
		int minkeystroke = 10000;

		// Try every combination of buttons
		for (unsigned int i = 0; i < 1 << keycount; i++) {
			unsigned int bits = 0;
			int bitcount = 0;
			int keynr = 0;
			int tmp = i;
			// Loop over buttons
			while(tmp) {
				// If presssed, then XOR lights
				if (tmp & 1) {
					bits ^= keys[keynr];
					bitcount++;
				}
				keynr++;
				tmp >>= 1;
			}
			// If lights on match target --> Hurray
			// Check if minimal
			if (bits == target) {
				if (bitcount < minkeystroke)
					minkeystroke = bitcount;
			}

		}
		// Add for all rows
		total += minkeystroke;
		
	}
	printf("Total: %d\n", total);
	
	return 0;
}

