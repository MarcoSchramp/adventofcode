#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[])
{
	char line[1024];
	char targetbuf[1024];
	int total = 0;
	while(scanf("%s", line) != EOF) {
		char *t = targetbuf;

		// Append opening quote
		*(t++) = '\"';
		for (char *ptr = line; *ptr; ptr++) {
			if (*ptr == '\"' ) {
				*(t++) = '\\';
			} 
			else if (*ptr == '\\') {
				*(t++) = '\\';
			}
			*(t++) = *ptr;
		}
		*(t++) = '\"';
		*t = 0;

		total += strlen(targetbuf) - strlen(line);
	}
	printf("save space: %d\n", total);
	return 0;
}
