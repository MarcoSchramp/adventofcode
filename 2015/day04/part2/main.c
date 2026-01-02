
#include "md5.h"
#include<stdio.h>

void print_hash(uint8_t *p){
    for(int i = 0; i < 16; ++i){
        printf("%02x", p[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
	uint8_t hash[16];
	char* input = "yzbqklnj";


	long i = 0;
	char buffer[100];
	do {
		snprintf(buffer, sizeof buffer ,"%s%ld", input, i);
		md5String(buffer, hash);
		if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0) {
			break;
		}
		i++;
		
	} while (1);

	printf("%ld --> %s --> ", i, buffer);
	print_hash(hash);

	return 0;
}
