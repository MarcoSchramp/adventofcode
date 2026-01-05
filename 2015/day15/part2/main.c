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

int calc_lastspoon(struct ingredient * ingredients, int* spoons, int ingredientcount)
{
	int calories= 0;
	for (int i = 0 ; i < ingredientcount - 1; i++)	
		calories += spoons[i]*ingredients[i].calories;
	int lastspoon = (500 - calories )/ ingredients[ingredientcount - 1].calories;
	return lastspoon;
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

	int atmax;
	memset(spoons, 0, sizeof spoons);
	spoons[ingredientcount] = calc_lastspoon(ingredients, spoons, ingredientcount);
	int besttaste = calc_taste(ingredients, spoons, ingredientcount);

	// brute-force it: loop over all combinaties (only 1000000, with many impossibilities)
	do {	
		// Cannot have negative amount of spoons
		if (spoons[ingredientcount - 1] >= 0) {
			int totalspoons = 0;
			for (int i = 0 ; i < ingredientcount; i++)
				totalspoons += spoons[i];
	
			// There must be 100 spoons used
			if (totalspoons == 100) {
				int taste = calc_taste(ingredients, spoons, ingredientcount);
				if (taste > besttaste)
						besttaste = taste;
			}
		}

		// Increase to the next
		atmax  = 0;
		while(atmax < ingredientcount - 1 && ++spoons[atmax] > 100 )
			spoons[atmax++] = 0;
		spoons[ingredientcount - 1] = calc_lastspoon(ingredients, spoons, ingredientcount);
	} while(atmax < ingredientcount - 1);

	printf("Best taste = %d\n", besttaste);
	return 0;
}
