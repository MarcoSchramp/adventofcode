
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAXCONNECTEDDEVICES 100
#define MAXLINE 1000

// Structure for keeping device information
struct device {
	struct device* next; // Linked list!
	char *name;
	// NULL terminated array of input devices
	struct device* inputdevices[MAXCONNECTEDDEVICES];
	// NULL terminated array of output devices
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

// find device. If not found, create device
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

// Find/create devices <from> and <to>
// Connect devices
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

// Naive recursive approach to count number of paths
int countpaths(struct device *device) {
	// Stop recursion at "out"
	if (!strcmp(device->name, "out"))
		return 1;

	// Recursion may also stop when a device has no outputs
	int count = 0;
	for(int i = 0; i < MAXCONNECTEDDEVICES && device->outputdevices[i]; i++)
		count += countpaths(device->outputdevices[i]);
	return count;
}

int main (int argc, char *argv[])
{
	char line[MAXLINE];
	while (fgets(line, sizeof line, stdin) != NULL) {
		char* strtok_handler1 = NULL;
		char* strtok_handler2 = NULL;
		// Get name before colon
		char* name = strtok_r(line, ":", &strtok_handler1);
		// Get outputs after colon
		char* outputs = strtok_r(NULL, ":\n", &strtok_handler1);

		// split outputs using " " and "\n" as separators
		for (char *outputname = strtok_r(outputs, " ", &strtok_handler2); outputname; outputname = strtok_r(NULL, " ", &strtok_handler2))
			connectdevices(name, outputname);
	}

	// Get the "you" device
	struct device* you = finddevice("you");

	printf ("nr of paths = %d\n", countpaths(you));
	return 0;
}
