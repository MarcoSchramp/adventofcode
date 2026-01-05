
#include "md5.h"
#include<stdio.h>

int main(int argc, char* argv[])
{
	uint8_t hash[16];
	char* input = "yzbqklnj";


	long i = 0;
	char buffer[100];
	do {
		snprintf(buffer, sizeof buffer ,"%s%ld", input, i);
		md5String(buffer, hash);
		if (hash[0] == 0 && hash[1] == 0 && (hash[2] & 0xF0) == 0) {
			break;
		}
		i++;
		
	} while (1);

	printf("%ld", i);

	return 0;
}
