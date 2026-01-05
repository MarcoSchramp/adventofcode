#include <stdio.h>

#define MAX_BUCKETS 30
#define TARGET 150

int buckets[MAX_BUCKETS];
int bucketcount = 0;
int minbuckets = MAX_BUCKETS;
int matchcount = 0;


void checkbuckets(int fill, int frompos, int cnt)
{
	cnt = cnt + 1;
	for ( /*no init*/; frompos < bucketcount; frompos++) {
		int newfill = fill+buckets[frompos];
		if (newfill == 150) {
			if (cnt < minbuckets) {
				minbuckets = cnt;
				matchcount = 1;
			}
			else if (cnt == minbuckets)
				matchcount++;
		}
		else if (newfill < 150) {	
			checkbuckets(newfill, frompos + 1, cnt);
		}
	}
}

int main(int argc, char* argv[]) 
{

	while (scanf("%d", buckets+bucketcount) != EOF)
		bucketcount++;
	checkbuckets(0, 0, 0);

	printf ("Buckets combination found %d\n", matchcount);
}
