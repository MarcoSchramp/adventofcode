#include <stdio.h>
#include <string.h>

struct ingredient {
	char name[100];
	int capacity;
	int durability;
	int flavor;
	int texture;
	int calories;
};

#define MAX_INGREDIENTS 10


int calc_taste(struct ingredient * ingredients, int* spoons, int ingredientcount)
{
	int capacity =0;
	int durability = 0;
	int flavor = 0;
	int texture = 0;
	for (int i = 0; i < ingredientcount; i++) {
		capacity   += spoons[i]*ingredients[i].capacity;
		durability += spoons[i]*ingredients[i].durability;
		flavor     += spoons[i]*ingredients[i].flavor;
		texture    += spoons[i]*ingredients[i].texture;
		
	}
	if (capacity <= 0 || durability <= 0 || flavor <= 0 || texture <= 0)
		return 0;
	return capacity * durability * flavor * texture;
}

int main(int argc, char* argv[])
{
	char line[1000];
	int spoons[MAX_INGREDIENTS];
	struct ingredient ingredients[MAX_INGREDIENTS];
	int ingredientcount = 0;
	while (fgets(line, sizeof line, stdin) != NULL) {
		sscanf (line, "%s capacity %d, durability %d, flavor %d, texture %d, calories %d",
			ingredients[ingredientcount].name,
			&ingredients[ingredientcount].capacity,
			&ingredients[ingredientcount].durability,
			&ingredients[ingredientcount].flavor,
			&ingredients[ingredientcount].texture,
			&ingredients[ingredientcount].calories);
		ingredientcount++;
	}

	// assume no local minimum, --> steepest descent.

	int atmax;
	memset(spoons, 0, sizeof spoons);

	// Let's start somewhere in the middle
	for (int i = 0; i < 100; i++)
		spoons[i % ingredientcount]++;
	int besttaste = calc_taste(ingredients, spoons, ingredientcount);

	do {
		atmax = 1;
		for (int i = 0 ; i < ingredientcount; i++)
			for (int j = 0; j < ingredientcount; j++) {
				if (i == j) continue;
				int testspoon[MAX_INGREDIENTS];
				memcpy(testspoon, spoons, sizeof testspoon);
				testspoon[i]++;
				testspoon[j]--;
				if (testspoon[j] < 0)
					continue;
				int taste = calc_taste(ingredients, testspoon, ingredientcount);
				if (taste > besttaste) {
					memcpy(spoons, testspoon, sizeof spoons);
					besttaste = taste;
					atmax = 0;
				}
			}
	} while(!atmax);

	printf("Best taste = %d\n", besttaste);
	return 0;
}
