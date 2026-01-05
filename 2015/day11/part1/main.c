#include<string.h>
#include<stdio.h>
#include<stdlib.h>

// Note the absence of i, o and l
char validchar[] = "abcdefghjkmnpqrstuvwxyz";
int maxvalidchar = 23;


void incrementpasswd(int* passwd, int length)
{
	while (length-- > 0) {
		if (++passwd[length] < maxvalidchar) 
			break;
		passwd[length] = 0;
	}
}

int hassequence (int* passwd, int length)
{
	for (int i = 0; i < length - 3; i++)
		if (validchar[passwd[i]]+1 == validchar[passwd[i+1]] &&
                    validchar[passwd[i]]+2 == validchar[passwd[i+2]] )
			return 1;
	return 0;
}

int* finddouble(int* passwd) 
{
	int* i = passwd;
	while (*i && *(i+1) && *i != *(i+1))
		i++;
	if (!*i || !*(i+1)) // Stopped due to zero termination
		return NULL;
	return i;
}

int* finddifferentdouble(int* passwd, int nomatch) 
{
	int* i = passwd;
	while (*i && *(i+1) && (*i != *(i+1) || *i == nomatch ))
		i++;
	if (!*i || !*(i+1)) // Stopped due to zero termination
		return NULL;
	return i;
}
 

int test(int* passwd, int length)
{
	if (!hassequence(passwd, length))
		return 0;
	int *fnd1 = finddouble(passwd);
	if (!fnd1)
		return 0;
	int *fnd2 = finddifferentdouble(fnd1 + 2, *fnd1);
	if (!fnd2)
		return 0;
	return 1;
}

int main(int argc, char* argv[])
{
	char initialpassword[] = "hxbxwxba";
	int passwdlength = strlen(initialpassword);

	int* passwdindex = malloc((passwdlength  + 1)* sizeof *passwdindex);
	for (int i = 0; i  < passwdlength; i++)
		passwdindex[i] = strchr(validchar, initialpassword[i]) - validchar;

	// zero terminated array :)
	passwdindex[passwdlength] = 0;
	
	do {
		incrementpasswd(passwdindex, passwdlength);
	} while (!test(passwdindex, passwdlength));

	for(int i = 0; i < passwdlength; i++)
		printf("%c",validchar[passwdindex[i]]);

	printf("\n");
	
	return 0;
}

