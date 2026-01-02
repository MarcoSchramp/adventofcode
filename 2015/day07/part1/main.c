
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define OP_NOT    1	/* 1 input value --> bitflip */
#define OP_AND    2	/* 2 input values --> AND operation */
#define OP_OR     3	/* 2 input values --> OR operation */
#define OP_LSHIFT 4	/* 2 input values --> bitwise shift left */
#define OP_RSHIFT 5	/* 2 input values --> bitwise shift right */
#define OP_ASSIGN 6	/* 1 input value unchanged */

#define MAX_OUT 100
struct node;

struct operation {
	int op;
	struct node* in1;
	struct node* in2;
	struct node* out;
};

struct node {
	struct node* next; // A linked list for simple lookup
	struct operation* in;
	struct operation* out[MAX_OUT]; // maximum number of connections out to other operations
	char* name;
	int value;
	int calculated;
};

struct node* nodelist;


struct node* find_node(char *name)
{
	if (name == NULL)
		return NULL;
	for(struct node* n = nodelist; n; n = n->next)
		if (!strcmp(n->name, name))
			return n;
	return NULL;
}

struct node* upsert_node(char *name)
{
	struct node* n = find_node(name);
	if (!n) {
		printf ("Node %s not found --> adding\n", name);
		n = malloc (sizeof *n);
		n-> in = NULL;
		memset(n->out, 0, sizeof n->out);
		n->name = strdup(name);
		n->value = 0;
		n->calculated = 0;
		n->next = nodelist;
		nodelist = n;
	}
	return n;
}

struct operation* new_operation(int op, struct node* in1, struct node* in2, struct node* out)
{
	struct operation* operation = malloc(sizeof *operation);
	operation->op = op;
	operation->in1 = in1;
	operation->in2 = in2;
	operation->out = out;
	
	out->in = operation;
	int i;
	if (in1) {
		for (i = 0; i < MAX_OUT && in1->out[i]; i++) ;
		in1->out[i] = operation;
	}

	if (in2) {
		for (i = 0; i < MAX_OUT && in2->out[i]; i++) ;
		in2->out[i] = operation;
	}

	return operation;
}

// forward declaration
long evalnode(struct node *n);

long evaloperation(struct operation *op)
{
	long i1 = 0;
	long i2 = 0;
	if (op->in1)
		i1 = evalnode(op->in1);
	if (op->in2)
		i2 = evalnode(op->in2);
	long o = 0;
	switch(op->op) {
		case OP_NOT: o = ~i1; break;
		case OP_AND: o = i1 & i2; break;
		case OP_OR: o = i1 | i2; break;
		case OP_LSHIFT: o = i1 << i2; break;
		case OP_RSHIFT: o = i1 >> i2; break;
		case OP_ASSIGN: o = i1; break;
		default: printf("Error evaluation\n"); break;
	}
	return o;
	
}

long evalnode(struct node *n)
{
	long o = 0;
	if (n->calculated)
		return n->value;
	if (n->in) o = evaloperation(n->in);
	else o = atoi(n->name);
	n->value = o;
	n->calculated = 1;
	return o;
}

int main(int argc, char* argv[])
{
	char line[1024];

	while (fgets(line, sizeof line, stdin) != NULL) {
		int op = 0;
		struct node* in1 = NULL;
		struct node* in2 = NULL;
		struct node* out = NULL;
	
		// First token must either be a token, or a NOT operation
		char* token = strtok(line, " \n");
		if (!strcmp(token, "NOT")) {
			printf("NOT!!\n");
			op = OP_NOT;
			token = strtok(NULL, " \n");	
		}

		in1 = upsert_node(token);
		printf("First variable = %s\n", in1->name);

		token = strtok(NULL, " \n");

		if (!strcmp(token, "->")) {
			if (op != OP_NOT)
				op = OP_ASSIGN;
		}
		else {
			if (!strcmp(token, "AND")) op = OP_AND;
			else if (!strcmp(token, "OR")) op = OP_OR;
			else if (!strcmp(token, "LSHIFT")) op = OP_LSHIFT;
			else if (!strcmp(token, "RSHIFT")) op = OP_RSHIFT;
			else printf ("Syntax error");

			// After operation must be another node reference
			token = strtok(NULL, " \n");
			in2 = upsert_node(token);
			printf("Second variable = %s\n", in2->name);

			// Then there must be a -> operation
			token = strtok(NULL, " \n");
			if (strcmp(token, "->"))
				printf ("Syntax error: -> expected");
		}

		token = strtok(NULL, " \n");
		out = upsert_node(token);
		printf("Third variable = %s\n", out->name);

		new_operation(op, in1, in2, out);
		
	}
	printf("start eval\n");

	struct node* a = find_node("a");
	printf ("Node a found: %d\n", a != NULL);

	printf("eval(a) = %ld\n", evalnode(a));

	return 0;
}
