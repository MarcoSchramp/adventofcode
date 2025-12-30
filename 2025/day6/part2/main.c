#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define LINELENGTH 10000

// line buffer
char *line[5];

// A sepatator column exists off vertical spaces
// The function returns the column number of the column after startcol
int findNextSeparatorColumn(int startcol)
{
	int maxlength = strlen(line[0]);
	int nextCol = startcol + 1;
	while ( nextCol < maxlength && (
		line[0][nextCol] != ' ' ||
		line[1][nextCol] != ' ' ||
		line[2][nextCol] != ' ' ||
		line[3][nextCol] != ' ' ||
		line[4][nextCol] != ' ' ))
		nextCol ++;
	return nextCol;
}

// find the operator at or after position "start"
char findOperator(int start)
{
	while (line[4][start] == ' ')
		start++;
	return line[4][start];
}

// Read the number in column col
long findNum(int col)
{
	char num[5];
	num[0] = line[0][col];
	num[1] = line[1][col];
	num[2] = line[2][col];
	num[3] = line[3][col];
	num[4] = 0;
	return atol(num);
}

int main(int argc, char* argv[])
{
	// Allocate memory for 5 line reads
	line[0] = malloc(LINELENGTH);
	line[1] = malloc(LINELENGTH);
	line[2] = malloc(LINELENGTH);
	line[3] = malloc(LINELENGTH);
	line[4] = malloc(LINELENGTH);

	// Read the lines
	fgets(line[0], LINELENGTH, stdin);
	fgets(line[1], LINELENGTH, stdin);
	fgets(line[2], LINELENGTH, stdin);
	fgets(line[3], LINELENGTH, stdin);
	fgets(line[4], LINELENGTH, stdin);

	// Remove newlines
	line[0][strlen(line[0])-1]= 0;
	line[1][strlen(line[1])-1]= 0;
	line[2][strlen(line[2])-1]= 0;
	line[3][strlen(line[3])-1]= 0;
	line[4][strlen(line[4])-1]= 0;

	// What we're looking for
	long total = 0;

	// Index that points to the first column that contains an exercise
	int start = 0;
	// loop until the "start" is at the end of the line
	while (start < strlen(line[0])) {
		// find where this exercise ends
		int nextsep = findNextSeparatorColumn(start);

		// Find the operator
		char operator = findOperator(start);	
		// Start value is 0 or 1, depending on operator
		long result = operator == '+' ? 0 : 1;

		// get each number and apply operator
		for (; start < nextsep; start++) {
			long num = findNum(start);
			result = operator == '+' ? result + num : result * num;
			
		}
		// Skip empty column
		start++;
		total += result;
		
	}

	printf("result: %ld\n", total);

	return 0;
}
