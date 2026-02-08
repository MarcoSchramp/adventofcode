#include<stdlib.h>
#include<stdio.h>
#include<string.h>


struct foundABA {
	struct foundABA* next;
	char aba[3];
};

struct foundABA* foundABAlist = NULL;
struct foundABA* foundBABlist = NULL;

void clearABAList(struct foundABA** list)
{
	while (*list) {
		struct foundABA* next = (*list)->next;
		free(*list);
		*list = next;
	}
}

void addABA(struct foundABA** list, char* aba)
{
	struct foundABA* foundABA = malloc(sizeof *foundABA);
	foundABA->next = *list;
	memcpy(foundABA->aba, aba, 3);

	*list = foundABA;
}

int match()
{
	for (struct foundABA* aba = foundABAlist; aba; aba = aba->next)
		for (struct foundABA* bab = foundBABlist; bab; bab = bab->next) {
			if (bab->aba[0] == aba->aba[1] && bab->aba[1] == aba->aba[0])
				return 1;
		}
	return 0;
}

int main(int argc, char* argv[])
{
	char line[1024];
	int count = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// remove trailing newline
		line[strlen(line) - 1] = 0;
	
		// No need to process very short lines, cannot be a match
		if (strlen(line) < 3)
			continue;
	

		int inbrackets = 0;
		
		for (char *p = line; *(p+2) != 0; p++) {
			if (*p == '[') inbrackets = 1;
			if (*p == ']') inbrackets = 0;
			if (*p == *(p+2) && *(p+1) != ']' && *(p+1) != '[') {
				if (inbrackets) addABA(&foundBABlist, p);
				else	        addABA(&foundABAlist, p);
			}
		}
		if (match())
			count++;
		clearABAList(&foundABAlist);
		clearABAList(&foundBABlist);
	}
	printf ("Nr of IPs with SSL: %d\n", count);

	return 0;
}
