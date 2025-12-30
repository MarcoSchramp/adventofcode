
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXCONNECTEDDEVICES 100
#define MAXLINE 1000

struct device {
	struct device* next;
	char *name;
	struct device* inputdevices[MAXCONNECTEDDEVICES];
	struct device* outputdevices[MAXCONNECTEDDEVICES];
};

struct device* devicelist = NULL;

// Simple linear search
struct device* finddevice(char *name)
{
	struct device* result = devicelist;
	while (result && strcmp(name, result->name))
		result = result -> next;
	return result;
}

struct device *upsertdevice(char *name)
{
	struct device* result = finddevice(name);
	if (!result) {
		result = malloc(sizeof *result);
		result->name = strdup(name);
		memset(result->inputdevices, 0, sizeof result->inputdevices);
		memset(result->outputdevices, 0, sizeof result->outputdevices);
		result->next = devicelist;
		devicelist = result;
	}
	return result;
}

void connectdevices(char *from, char *to)
{
	struct device* fromdevice = upsertdevice(from);
	struct device* todevice = upsertdevice(to);
	int i;
	for (i = 0; i < MAXCONNECTEDDEVICES && todevice->inputdevices[i]; i++);
	assert(i < MAXCONNECTEDDEVICES);
	todevice->inputdevices[i] = fromdevice;

	for (i = 0; i < MAXCONNECTEDDEVICES && fromdevice->outputdevices[i]; i++);
	assert(i < MAXCONNECTEDDEVICES);
	fromdevice->outputdevices[i] = todevice;
}

int countpaths(struct device *device) {
	printf ("Counting %s\n", device->name);
	if (!strcmp(device->name, "out"))
		return 1;
	int count = 0;
	for(int i = 0; i < MAXCONNECTEDDEVICES && device->outputdevices[i]; i++)
		count += countpaths(device->outputdevices[i]);
	return count;
}

int main (int argc, char *argv[])
{
	char line[MAXLINE];
	while (fgets(line, sizeof line, stdin) != NULL) {
		printf ("%s", line);
		char* strtok_handler1 = NULL;
		char* strtok_handler2 = NULL;
		char* name = strtok_r(line, ":", &strtok_handler1);
		char* outputs = strtok_r(NULL, ":\n", &strtok_handler1);
		for (char *outputname = strtok_r(outputs, " ", &strtok_handler2); outputname; outputname = strtok_r(NULL, " ", &strtok_handler2))
			connectdevices(name, outputname);
	}

	printf ("-----------------------\n");
	for (struct device* d = devicelist; d; d =d->next) {
		printf ("%s:", d->name);
		for(int i = 0; i < MAXCONNECTEDDEVICES && d->outputdevices[i]; i++)
			printf(" %s", d->outputdevices[i]->name);
		printf("\n");
	}
	printf ("-----------------------\n");
		
	

	struct device* you = finddevice("you");
	printf ("nr of paths = %d\n", countpaths(you));
	return 0;
}
