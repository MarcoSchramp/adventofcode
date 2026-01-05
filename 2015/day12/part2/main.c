
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//Assume wellformed JSON

#define TOKEN_ARRAYSTART	0
#define TOKEN_ARRAYEND		1
#define TOKEN_OBJECTSTART	2
#define TOKEN_OBJECTEND		3
#define TOKEN_NUMBER		4
#define TOKEN_TEXT		5
#define TOKEN_COLON		6
#define TOKEN_COMMA		7
#define TOKEN_ERROR		8


// Lexical analyzer: produces valid tokens
int nextToken(char *buffer)
{
	char *s = buffer;
	int c = getchar();
	int token = TOKEN_ERROR;
	*(buffer++) = (char) c;
	if (c == '[') token = TOKEN_ARRAYSTART;
	else if (c == ']') token = TOKEN_ARRAYEND;
	else if (c == '{') token = TOKEN_OBJECTSTART;
	else if (c == '}') token = TOKEN_OBJECTEND;
	else if (c == ':') token = TOKEN_COLON;
	else if (c == ',') token = TOKEN_COMMA;
	else if (c == '\"') {
		while ((c = getchar()) != '\"')
			*(buffer++) =c;
		// Add closing quotes
		*(buffer++) =c;
		token = TOKEN_TEXT;
	}
	else if (c == '-' || strchr("0123456789", c)) {
		do {
			c = getchar();
			*(buffer++) =c;
		} while (strchr("0123456789", c));
		// Push last character back into read-buffer
		ungetc(c, stdin);
		buffer--;
		token = TOKEN_NUMBER;
		
	}
		
	*(buffer++) = 0;

	if (token == TOKEN_ERROR)
		printf ("token: %d : %s\n", token, s);
	return token;
}

// forward declaration
int parseObject(char* buffer, int);

int parseArray(char* buffer, int level)
{
	int sum = 0;
	int token;
	while ((token = nextToken(buffer)) != TOKEN_ARRAYEND) {
		// Skip comma
		if (token == TOKEN_COMMA)
			token = nextToken(buffer);

		if (token == TOKEN_ARRAYSTART)
			sum += parseArray(buffer, level + 1);
		if (token == TOKEN_OBJECTSTART)
			sum += parseObject(buffer, level + 1);
		else if (token ==  TOKEN_TEXT) {
			// ignore
		}
		else if (token == TOKEN_NUMBER) {
			sum += atoi(buffer);
		}
	}
	return sum;
}
int parseObject(char *buffer, int level)
{
	int token;
	int redfound = 0;
	int sum = 0;
	

	while ((token = nextToken(buffer)) != TOKEN_OBJECTEND) {
		// Skip comma
		if (token == TOKEN_COMMA)
			token = nextToken(buffer);
		if (token != TOKEN_TEXT) printf ("Expected text\n");
		token = nextToken(buffer);
		if (token != TOKEN_COLON) printf ("Expected colon\n");
		token = nextToken(buffer);

		if (token == TOKEN_ARRAYSTART)
			sum += parseArray(buffer, level + 1);
		else if (token == TOKEN_OBJECTSTART)
			sum += parseObject(buffer, level + 1);
		else if (token ==  TOKEN_TEXT) {
			// Check if red!
			if (strcmp(buffer, "\"red\"") == 0)
				redfound = 1;
		}
		else if (token == TOKEN_NUMBER) {
			sum += atoi(buffer);
		}
	}
	return redfound ? 0 : sum;
}


int main(int argc, char* argv[])
{
	char buffer[100];
	int token = nextToken(buffer);
	int total = token == TOKEN_ARRAYSTART ? parseArray(buffer, 0) : parseObject(buffer, 0);

	printf("Totaal = %d\n", total);
	return 0;
}
