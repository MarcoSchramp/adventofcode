
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Some limits to make coding easier
#define MAXCONNECTEDDEVICES 100
#define MAXLINE 1000

// 2 bit marking
#define MARK_FFT 0x1
#define MARK_DAC 0x2
#define MARK_ALL (MARK_FFT | MARK_DAC)

struct device {
	// A linked list to keep the collection searchable
	struct device* next;

	// Every device has a name
	char *name;

	// The device gets its inputs from these devices : it's a NULL terminated array
	struct device* inputdevices[MAXCONNECTEDDEVICES];
	// The device gets its outputs from these devices : it's a NULL terminated array
	struct device* outputdevices[MAXCONNECTEDDEVICES];

	// We do a sweep to mark devices that receive or are input to FFT or DAC.
	int mark;

	// This is  cache value (-1 means: must be calculated
	long count;
};

// Root of linked list
struct device* devicelist = NULL;

// Simple linear search
struct device* finddevice(char *name)
{
	struct device* result = devicelist;
	while (result && strcmp(name, result->name))
		result = result->next;
	return result;
}

// Return the device when it exists, Add the device when not known,
struct device *upsertdevice(char *name)
{
	struct device* result = finddevice(name);
	if (!result) {
		result = malloc(sizeof *result);
		result->name = strdup(name);
		memset(result->inputdevices, 0, sizeof result->inputdevices);
		memset(result->outputdevices, 0, sizeof result->outputdevices);
		result->next = devicelist;
		result->mark = 0;
		result->count = -1;
		devicelist = result;
	}
	return result;
}

// Connect two devices (create devices when these don't exist)
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

// Mark strategy to mark inputs or outputs of a device
void mark(struct device* device, int up, int marker)
{
	printf("Marking: %s\n", device->name);
	// Indien mark al gezet, dan overslaan
	if (device->mark & marker)
		return;
	device->mark |= marker;

	struct device** devices = up ? device->inputdevices : device->outputdevices;
	for (int i = 0; (i < MAXCONNECTEDDEVICES) && devices[i]; i++)
		mark(devices[i], up, marker);
}

// Count the number of paths to this device
// Cache the result, in order to prevent walking the tree again.
long count(struct device* device)
{
	// If counted before, return that cached result
	if (device->count >= 0)
		return device->count;

	// When not all marks, then stop.
	if ((device->mark & MARK_ALL) != MARK_ALL) {
		device->count = 0;
		return 0;
	}

	// Root node always returns 1
	if (!strcmp(device->name, "svr")) {
		device->count = 1;
		return 1;
	}

	// Sum it up and cache the result
	long counter = 0;
	for (int i = 0; i < MAXCONNECTEDDEVICES && device->inputdevices[i]; i++)
		counter += count(device->inputdevices[i]);
	device->count = counter;
	return counter;
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
		
	

	struct device* fft = finddevice("fft");
	struct device* dac = finddevice("dac");

	// Some trickery with the top-node of marking: the algoritm marks the node
	// thereby preventing the second run. Therefore the mark is cleared between
	// the first and second call
	mark(fft, 1, MARK_FFT);
	fft->mark = 0;
	mark(fft, 0, MARK_FFT);
	fft->mark = MARK_FFT;
	
	int tmp = dac->mark;
	mark(dac, 1, MARK_DAC);
	dac->mark = 0;
	mark(dac, 0, MARK_DAC);
	dac->mark = tmp | MARK_DAC;
	

	// Now count the number of paths that read "out"
	struct device* out = finddevice("out");
	printf ("nr of paths = %ld\n", count(out));
	return 0;
}
