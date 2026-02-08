#include <stdio.h>


int keys[7][7] = {
	{ 0,   0,   0,  0,    0,   0, 0},
	{ 0,   0,   0, '1',   0,   0, 0},
	{ 0,   0, '2', '3', '4',   0, 0},
	{ 0, '5', '6', '7', '8', '9', 0},
	{ 0,   0, 'A', 'B', 'C',   0, 0},
	{ 0,   0,   0, 'D',   0,   0, 0},
	{ 0,   0,   0,   0,   0,   0, 0}
};


int main(int argc, char* argv[])
{
	int c;
	int fingerposx = 1;
	int fingerposy = 3;

	while ((c = fgetc(stdin)) != EOF) {
		switch(c) {
			case 'U': fingerposy = keys[fingerposy - 1][fingerposx    ] == 0 ? fingerposy : fingerposy - 1; break;
			case 'D': fingerposy = keys[fingerposy + 1][fingerposx    ] == 0 ? fingerposy : fingerposy + 1; break;
			case 'L': fingerposx = keys[fingerposy    ][fingerposx - 1] == 0 ? fingerposx : fingerposx - 1; break;
			case 'R': fingerposx = keys[fingerposy    ][fingerposx + 1] == 0 ? fingerposx : fingerposx + 1; break;
			case '\n': putc(keys[fingerposy][fingerposx], stdout); break;
		}
		
	}
	printf("\n");
	return 0;
}
