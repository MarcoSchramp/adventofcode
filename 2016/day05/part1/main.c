
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "md5.h"

int main(int argc, char* argv[])
{
	char prefix[] = "wtnhxymk";
	char buffer[1024];
	int count = 0;

	long i = 0;
	while (count < 8) {
		uint8_t md5hash[16];
		sprintf(buffer, "%s%ld", prefix, i++);
		md5String(buffer, md5hash);
		// First 5 nibbles are 0
		if (md5hash[0] == 0 && md5hash[1] == 0 && (md5hash[2] & 0xF0) == 0) {
			printf("%x", md5hash[2]);
			count++;
		}
	}

	return 0;
}
