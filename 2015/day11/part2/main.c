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

int finddouble(int* passwd, int length) 
{
	int pos = 0;
	while (pos < length - 1 && passwd[pos]!= passwd[pos+1])
		pos++;
	if (pos == length - 1) // Stopped due to zero termination
		return -1;
	return pos;
}

int finddifferentdouble(int* passwd, int length, int nomatch) 
{
	if (length < 0)
		return -1;
	int pos = 0;
	while (pos < length -1 && (passwd[pos]!= passwd[pos+1] || passwd[pos]==nomatch)) {
		pos++;
	}
	if (pos >= length - 1) // Stopped due to zero termination
		return -1;
	return pos;
}
 

int test(int* passwd, int length)
{
	if (!hassequence(passwd, length))
		return 0;

	int fnd1 = finddouble(passwd, length);
	if (fnd1 < 0)
		return 0;
	int fnd2 = finddifferentdouble(passwd + fnd1 + 2, length - fnd1 - 2, passwd[fnd1]);
	if (fnd2 < 0)
		return 0;
	return 1;
}

int main(int argc, char* argv[])
{
	char initialpassword[] = "hxbxxyzz";
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

