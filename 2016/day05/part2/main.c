
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "md5.h"

int main(int argc, char* argv[])
{
	char prefix[] = "wtnhxymk";
	char buffer[1024];
	int exitloop = 1;
	char resultbuffer[9];

	memset(resultbuffer, 0, sizeof resultbuffer);

	long i = 0;
	while (exitloop) {
		uint8_t md5hash[16];
		sprintf(buffer, "%s%ld", prefix, i++);
		md5String(buffer, md5hash);
		// First 5 nibbles are 0

		if (md5hash[0] == 0 && md5hash[1] == 0 && (md5hash[2] & 0xF0) == 0) {
			int pos = md5hash[2];
			int value = (md5hash[3] & 0xF0 ) >> 4;
			if (pos < 8 && resultbuffer[pos] == 0) {
				resultbuffer[pos] = value < 10 ? '0' + value : 'A' + value - 10;

			}
			for (int j = 0 ; j < 8; j++)
				printf("%c", resultbuffer[j] ? resultbuffer[j] : '_');
			printf ("\n");

			if (strlen(resultbuffer) == 8)
				exitloop = 0;
		}
	}

	return 0;
}
