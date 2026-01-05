#include<stdio.h>
#include<string.h>

int main(int argc, char *argv[])
{
	char line[1024];
	int total = 0;
	while(scanf("%s", line) != EOF) {
		int linelength = strlen(line);

		int charcount = 0;
		char *ptr = line;
		while (*ptr) {
			if (*ptr == '\"' && (ptr == line || *(ptr + 1) == 0)) {
				// skip opening/closing double quote
				ptr++;
			} 
			else if (*ptr == '\\' && *(ptr+1) == '\\') {
				charcount++;
				ptr = ptr + 2;
			}
			else if (*ptr == '\\' && *(ptr+1) == '\"') {
				charcount++;
				ptr = ptr + 2;
			}
			else if (*ptr == '\\' && *(ptr+1) == 'x') {
				// For now we don't care which character
				charcount++;
				ptr = ptr + 4;
			}
			else {
				// For now we don't care which character
				charcount++;
				ptr++;
			}
		}
		total += linelength - charcount;
	}
	printf("save space: %d\n", total);
	return 0;
}
