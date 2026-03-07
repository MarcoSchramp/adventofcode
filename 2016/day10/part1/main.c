
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct bot {
	struct bot* next;
	int botno;
	int lowto; // If lowto < 0 --> lowto is output, otherwise botno
	int highto; // if highto < 0 --> highto is output, otherwise botno

	int chip1;
};

struct load{
	struct load* next;
	int chipno;
	int botno;
};


struct bot* botlist = NULL;
struct load* loadlist = NULL;

struct bot* findbot(int botno) {
	struct bot* result = botlist;
	while (result && result->botno != botno)
		result = result->next;
	return result;
}

int multiplyanswer = 1;

void loadbotwithchip(struct bot* bot, int chipid, int level)
{
	if (bot->chip1 < 0)
		bot->chip1 = chipid;
	else {

		if (chipid == 17 && bot->chip1 == 61)
			printf ("Found bot: %d\n", bot->botno);
		if (chipid == 61 && bot->chip1 == 17)
			printf ("Found bot: %d\n", bot->botno);
		// output will not return a struct bot* reference!
		struct bot* lowbot = findbot(bot->lowto);
		struct bot* highbot = findbot(bot->highto);
		if (bot->chip1 < chipid) {
			if (lowbot)
				loadbotwithchip(lowbot,bot->chip1, level + 1);
			else
				if (bot->lowto >= -3) multiplyanswer *= bot->chip1;
			if (highbot)
				loadbotwithchip(highbot, chipid, level + 1);
			else
				if (bot->highto >= -3) multiplyanswer *= chipid;
		}
		else {
			if (lowbot)
				loadbotwithchip(lowbot, chipid, level + 1);
			else
				if (bot->lowto >= -3) multiplyanswer *= chipid;
			if (highbot)
				loadbotwithchip(highbot, bot->chip1, level + 1);
			else
				if (bot->highto >= -3) multiplyanswer *= bot->chip1;
		}
		// Empty again
		bot->chip1 = -1;
	}
}


int main (int argc, char* argv[])
{
	char line[1024];
	struct load** appender = &loadlist;

	while (fgets(line, sizeof line, stdin) != NULL) {
		char * first = strtok(line, " \n");
		if (!strcmp(first, "bot")) {
			// parse instruction
			char* botno = strtok(NULL, " \n");
			strtok(NULL, " \n"); // skip "gives"
			strtok(NULL, " \n"); // skip "low"
			strtok(NULL, " \n"); // skip "to"
			char *botoutputlow =  strtok(NULL, " \n");
			char *lowvalue =  strtok(NULL, " \n");
			strtok(NULL, " \n"); // skip "and"
			strtok(NULL, " \n"); // skip "high"
			strtok(NULL, " \n"); // skip "to"
			char *botoutputhigh =  strtok(NULL, " \n");
			char *highvalue =  strtok(NULL, " \n");

			// setup bot

			struct bot* newbot = malloc(sizeof *newbot);
			newbot->next = botlist;
			newbot->botno = atoi(botno);
			newbot->lowto = strcmp(botoutputlow, "bot") ? -atoi(lowvalue) - 1: atoi(lowvalue);
			newbot->highto = strcmp(botoutputhigh, "bot") ? -atoi(highvalue) - 1: atoi(highvalue);
			newbot->chip1 = -1;
			botlist = newbot;
			
		}
		if (!strcmp(first, "value")) {
			char *chipno = strtok(NULL, " \n");
			strtok(NULL, " \n"); // skip "goes"
			strtok(NULL, " \n"); // skip "to"
			strtok(NULL, " \n"); // skip "bot"
			char* botno = strtok(NULL, " \n"); 

			// setup load instruction
			struct load* newload =  malloc (sizeof *newload);
			newload->next = NULL;
			newload->chipno = atoi(chipno);
			newload->botno = atoi(botno);
			*appender = newload;
			appender = &newload->next;
		}
	}

	// run the simulation
	for (struct load* load = loadlist; load; load= load->next) {
		struct bot* bot = findbot(load->botno);
		loadbotwithchip(bot, load->chipno, 0);
	}
	printf ("multiplyanswer = %d\n", multiplyanswer);
	return 0;
}
