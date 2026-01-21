
#include <stdio.h>
#include <string.h>

#define MAXPACKAGE 100


long weights[MAXPACKAGE];
long used[MAXPACKAGE];
long totalweight = 0;
long equalweight = 0;
long pkgcount = 0;

long frontcount = 1000;
long long frontentanglement = 2000000000000000;

int evalcount = 0;

void addbagthree(long* used)
{
	// There is no need to fit, the remaining have the right
	// weight and are marked as unused (0) in the used list
	long count = 0;
	long long entanglement = 1;
	for (long i = 0; i < pkgcount; i++) {
		if (used[i]) {
			count++;
			entanglement *= weights[i];
		}
	}
	if (frontcount > count) {
		frontcount = count;
		frontentanglement = entanglement;
	}
	else if (frontcount == count && frontentanglement > entanglement) {
		frontentanglement = entanglement;
	}
		
}

void addbagone(long* used, long inbag, long weight)
{
	for (long pkg = inbag; pkg < pkgcount; pkg++) {
		used[pkg] = 1;
		int newweight = weight + *(weights + pkg);
		if (newweight < equalweight)
			addbagone(used, pkg + 1, newweight);
		if (newweight == equalweight) 
			addbagthree(used);
		used[pkg] = 0;
	}
}

int main(int argc, char* argv[])
{


	memset (weights, 0, sizeof weights);
	memset (used, 0, sizeof used);

	while (scanf("%ld", weights+pkgcount) != EOF) {	
		totalweight += *(weights+pkgcount);
		pkgcount++;
	}
	equalweight = totalweight / 4;

	addbagone(used, 0, 0);

	printf ("Entanglement = %lld\n", frontentanglement);
	
	return 0;
}
