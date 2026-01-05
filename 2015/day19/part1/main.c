#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXMOLECULELENGTH 2048

struct molecule {
	struct molecule* next;
	char* formula;
};

struct modification {
	struct modification* next;
	char *mod;
};

struct modifier {
	struct modifier* next;
	struct modification *mod;
	char *pattern;
};

struct modifier * modifierlist = NULL;
struct molecule * moleculelist = NULL;

struct modifier* upsertmodifier(char *pattern)
{
	struct modifier** modifier = &modifierlist;
	while (*modifier && strcmp((*modifier)->pattern, pattern))
		modifier = &(*modifier)->next;
	if (!*modifier) {
		*modifier = malloc(sizeof **modifier);
		(*modifier)->next = NULL;
		(*modifier)->mod = NULL;
		(*modifier)->pattern = strdup(pattern);
	}
	return *modifier;
}

struct modification* upsertmodification(struct modifier* modifier, char *mod)
{
	struct modification** modification = &modifier->mod;
	while (*modification && strcmp((*modification)->mod, mod))
		modification = &(*modification)->next;
	if (!*modification) {
		*modification = malloc (sizeof **modification);
		(*modification)->next = NULL;
		(*modification)->mod = strdup(mod);
	}
	return *modification;
}

struct molecule* upsertmolecule(char *formula)
{
	struct molecule** molecule = &moleculelist;
	while (*molecule && strcmp((*molecule)->formula, formula))
		molecule = &(*molecule)->next;
	if (!*molecule) {
		*molecule = malloc(sizeof **molecule);
		(*molecule)->next = NULL;
		(*molecule)->formula = strdup(formula);
	}
	return *molecule;
}

int main(int arcg, char* argv[]) {
	char line[1000];
	char startmolecule[1000];
	int readmodifications = 1;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// remove terminating newline (if any)
		if (strchr(line, '\n')) *strchr(line, '\n') = 0;
		if (!strcmp(line, ""))
			readmodifications = 0;
		else if (readmodifications) {
			char modifierpattern[100];
			char modificationmod[100];
			sscanf(line, "%s => %s", modifierpattern, modificationmod);
			struct modifier* modifier = upsertmodifier(modifierpattern);
			upsertmodification(modifier, modificationmod);
		}
		else {
			strcpy(startmolecule, line);
		}
	}

	int processingpos = 0;
	while(startmolecule[processingpos]) {

		// Run through all possible modifications
		for (struct modifier* modifier = modifierlist; modifier; modifier = modifier->next) {

			// Check if the formula starts with the pattern an position processingpos
			if (!strncmp(modifier->pattern, startmolecule + processingpos, strlen(modifier->pattern))) {

			
				// Run through all possible modifications
				for(struct modification *mod = modifier->mod; mod; mod = mod->next) {
					
					char targetformula[MAXMOLECULELENGTH];
					sprintf(targetformula, "%.*s%s%s",
					    processingpos, startmolecule,
					    mod->mod,
					    startmolecule + processingpos + strlen(modifier->pattern));
					upsertmolecule(targetformula);
				}
			}
		}
		processingpos ++;
	}

	// write & destroy
	int count = 0;
	while(moleculelist) {
		count++;
		struct molecule* n = moleculelist->next;
		free(moleculelist->formula);
		free(moleculelist);
		moleculelist = n;
	}
	printf ("Found: %d\n", count);
}
