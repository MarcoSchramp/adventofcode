#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct attribute {
	struct attribute* next;
	char name[200];
	int count;
};
struct aunt {
	struct aunt* next;
	int id;
	struct attribute* attribute;
};

struct aunt * auntlist = NULL;

struct criterium {
	char *name;
	int count;
};

int match(struct criterium* criterium, struct aunt* aunt)
{
	for (struct attribute* attr = aunt->attribute; attr; attr = attr->next) {
		if ((!strcmp(criterium->name, "cats") ||
		     !strcmp(criterium->name, "trees")) &&
		    !strcmp(attr->name, criterium->name) &&
		    attr->count <= criterium->count)
			return 0;
		if ((!strcmp(criterium->name, "popmeranians") ||
		     !strcmp(criterium->name, "goldfish")) &&
		    !strcmp(attr->name, criterium->name) &&
		    attr->count >= criterium->count)
			return 0;
		if (strcmp(criterium->name, "cats") &&
		    strcmp(criterium->name, "trees") &&
		    strcmp(criterium->name, "popmeranians") &&
		    strcmp(criterium->name, "goldfish") &&
		    !strcmp(attr->name, criterium->name) &&
		    attr->count != criterium->count)
			return 0;
	}
	return 1;
}

int main(int argc, char* argv[])
{
	char line[1024];
	struct criterium criteria[] = {
		{ "children", 3 },
		{ "cats",  7 },
		{ "samoyeds",  2 },
		{ "pomeranians",  3 },
		{ "akitas",  0 },
		{ "vizslas",  0 },
		{ "goldfish",  5 },
		{ "trees",  3 },
		{ "cars",  2 },
		{ "perfumes",  1 }
	};

	while (fgets(line, sizeof line, stdin) != NULL) {
		char separator[] = " :,\n";
		strtok(line, separator); // ignore (text is "Sue")
		char *sueid = strtok(NULL, separator);
		struct aunt* aunt = malloc (sizeof *aunt);
		aunt->id = atoi(sueid);
		aunt->attribute = NULL;

		// Append to list
		aunt->next = auntlist;
		auntlist= aunt;

		char *attrname;
		char *attrvalue;
		while ((attrname = strtok(NULL, separator)) != NULL)  {
			attrvalue = strtok(NULL, separator);
			struct attribute* attr = malloc (sizeof *attr);
			strcpy(attr->name, attrname);
			attr->count = atoi(attrvalue);

			// Append to list
			attr->next = aunt->attribute;
			aunt->attribute = attr;
		}
	}


	for (int i = 0; i < sizeof criteria / sizeof *criteria; i++) {

		struct aunt** aunt = &auntlist;
		while (*aunt) {
			if (!match(criteria+i, *aunt))
				// Remove from list
				*aunt = (*aunt)->next;
			else
				// Next in list
				aunt = &(*aunt)->next;
		}
	}

	for (struct aunt* aunt = auntlist; aunt; aunt = aunt->next)
		printf("Matched aunt: %d\n", aunt->id);

	return 0;
}
