#include <stdio.h>


int keys[3][3] = {{1,2,3},{4,5,6}, {7,8,9}};


int main(int argc, char* argv[])
{
	int c;
	int fingerposx = 1;
	int fingerposy = 1;

	while ((c = fgetc(stdin)) != EOF) {
		switch(c) {
			case 'U': fingerposy = fingerposy == 0 ? 0 : fingerposy - 1; break;
			case 'D': fingerposy = fingerposy == 2 ? 2 : fingerposy + 1; break;
			case 'L': fingerposx = fingerposx == 0 ? 0 : fingerposx - 1; break;
			case 'R': fingerposx = fingerposx == 2 ? 2 : fingerposx + 1; break;
			case '\n': printf("%d", keys[fingerposy][fingerposx]);
		}
		
	}
	printf("\n");
	return 0;
}
