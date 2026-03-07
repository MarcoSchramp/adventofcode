#include "md5.h"


char * prefix = "ngcjuoqr";
//char * prefix = "abc";


struct foundkey {
	uint8_t key[16];	
	int c3;
	int c5;
};


// Triplet, but not a quadruplet or longer!
int findtriplet(const uint8_t hash[16])
{

	for (int i = 0; i< 15; i++) {
		// First test highest nibble
		if ((hash[i] & 0xF0) == (hash[i+1] & 0xF0) &&
                    (hash[i] & 0xF0) == (hash[i]   & 0x0F) << 4)
			return hash[i] &0x0F;
		// Then the lowest nibble
		if ((hash[i]   & 0x0F)  == (hash[i+1] & 0x0F) &&
                    (hash[i+1] & 0xF0)  == (hash[i+1] & 0x0F) << 4)
			return hash[i+1] &0x0F;
	}
	return -1; // Not found
}

int findquintuplet(const uint8_t hash[16])
{
	for (int i = 0; i< 14; i++) {
		// First test highest nibble
		if ((hash[i] & 0xF0) == (hash[i+1] & 0xF0) &&
		    (hash[i] & 0xF0) == (hash[i+2] & 0xF0) &&
                    (hash[i] & 0xF0) == (hash[i]   & 0x0F) << 4 &&
                    (hash[i] & 0xF0) == (hash[i+1]   & 0x0F) << 4)
			return hash[i] &0x0F;
		// Then the lowest nibble
		if ((hash[i]   & 0x0F)  == (hash[i+1] & 0x0F) &&
		    (hash[i]   & 0x0F)  == (hash[i+2] & 0x0F) &&
                    (hash[i]   & 0x0F)  == (hash[i+1] & 0xF0) >> 4 &&
                    (hash[i]   & 0x0F)  == (hash[i+2] & 0xF0) >> 4)
			return hash[i+1] &0x0F;
	}
	return -1; // Not found
}

char* genMD5hex(uint8_t hash[16])
{
	static char md5string[33];
	for (int i = 0; i < 16; i++)
		sprintf (md5string + 2*i, "%2.2x", hash[i]);
	return md5string;
}
char* genMD5hex2(uint8_t hash[16])
{
	static char md5string[33];
	for (int i = 0; i < 16; i++)
		sprintf (md5string + 2*i, "%2.2x", hash[i]);
	return md5string;
}


#define WINDOWSIZE   1000
#define REPEATCOUNT  2016
struct foundkey keyarray[WINDOWSIZE];

int main(int argc, char *argv[])
{
	int pos = 0;

	// Initialize first WINDOWSIZE hashes
	for (pos = 0; pos < WINDOWSIZE; pos++) {
		char keybuffer[100];
		sprintf(keybuffer, "%s%d", prefix, pos);

		md5String(keybuffer, keyarray[pos].key);
		for(int rep = 0; rep < REPEATCOUNT; rep++) {
			strcpy(keybuffer, genMD5hex(keyarray[pos].key));
			md5String(keybuffer, keyarray[pos].key);
		}
		keyarray[pos].c3 = findtriplet(keyarray[pos].key);
		keyarray[pos].c5 = findquintuplet(keyarray[pos].key);
		printf ("key %d : %s (%2d %2d)\n", pos, genMD5hex(keyarray[pos].key), keyarray[pos].c3, keyarray[pos].c5);
	}

	int matchcount = 0;
	while (matchcount < 64) {
		for (int i = 1; i < WINDOWSIZE; i++)
		{
			if (keyarray[pos % WINDOWSIZE].c3 == keyarray[(pos+i) % WINDOWSIZE].c5 && keyarray[pos % WINDOWSIZE].c3 >= 0) {
				matchcount ++;
				printf ("Match count %d at %d : %s (%x) and %s (%x)\n", matchcount, pos - WINDOWSIZE,
					genMD5hex(keyarray[pos % WINDOWSIZE].key),
					keyarray[pos % WINDOWSIZE].c3,
					genMD5hex2(keyarray[(pos + i) % WINDOWSIZE].key),
					keyarray[(pos + i) % WINDOWSIZE].c5);
				break;
			}
		}

	
		char keybuffer[100];
		sprintf(keybuffer, "%s%d", prefix, pos);

		md5String(keybuffer, keyarray[pos % WINDOWSIZE].key);
		for(int rep = 0; rep < REPEATCOUNT; rep++) {
			strcpy(keybuffer, genMD5hex(keyarray[pos % WINDOWSIZE].key));
			md5String(keybuffer, keyarray[pos % WINDOWSIZE].key);
		}

		keyarray[pos % WINDOWSIZE].c3 = findtriplet(keyarray[pos % WINDOWSIZE].key);
		keyarray[pos % WINDOWSIZE].c5 = findquintuplet(keyarray[pos % WINDOWSIZE].key);

		pos++;
	}

	return 0;
	
}
