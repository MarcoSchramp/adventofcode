#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000
#define MAXBUTTON 30
#define MAXLIGHT 30

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	int nextbutton=0;
	int total = 0;
	int linecount  = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		linecount++;

		//
		int matrix[MAXBUTTON][MAXLIGHT];
		int origmatrix[MAXBUTTON][MAXLIGHT];
		int swapcolumns[MAXBUTTON];

		int lightcount = 0;
		int buttoncount = 0;
		memset(matrix, 0, sizeof matrix);
		int maxjoltage = 0;

		char *strtok_space = NULL;
		for (char *part = strtok_r(line, " ", &strtok_space);
		     part ; part = strtok_r(NULL, " ", &strtok_space)) {
			if (*part == '[') {
				part++;
				while (*(part++) != ']') {
					lightcount++;
				}
			}
					
			if (*part == '(') {
				char *strtok_idx = NULL;
				for(char *idx = strtok_r(part + 1, ",)", &strtok_idx);
				    idx; idx = strtok_r(NULL, ",)", &strtok_idx)) {
					int x = atoi(idx);
					matrix[x][buttoncount] = 1;
				}
				swapcolumns[buttoncount] = buttoncount;
				buttoncount++;
			}
			if (*part == '{') {
				// Add column to the right
				char *strtok_idx = NULL;
				int light = 0;
				for(char *idx = strtok_r(part + 1, ",}", &strtok_idx);
				    idx; idx = strtok_r(NULL, ",}", &strtok_idx)) {
					int requiredjoltage = atoi(idx);
					matrix[light][buttoncount] = requiredjoltage;
					if (requiredjoltage > maxjoltage) maxjoltage = requiredjoltage;
					light++;
				}
			}
		}

		for (int button = 0; button < buttoncount; button++) {
			int minvalue = 100000;
			for (int light = 0; light < lightcount; light++) {
				if (matrix[light][button] && (matrix[light][buttoncount] < minvalue))
					minvalue = matrix[light][buttoncount];
			}
		}

		// Keep a copy for validation later
		memcpy(origmatrix, matrix, sizeof matrix);

		// do gaussian elimination
		int swapcolumn = buttoncount;
		int rank = lightcount;
		for (int row =0; row < lightcount; row++) {
			while (row < swapcolumn && row < lightcount && matrix[row][row] == 0)  {

				// Find a row below without 0 in the same column
				int switchrow;
				for(switchrow = row + 1; switchrow < lightcount; switchrow++) {
					if (matrix[switchrow][row] != 0) break;
				}

				// swap this with another row
				if (switchrow != lightcount) {
					// swap rows
					for (int i = 0; i < buttoncount + 1; i++) {
						int tmp = matrix[row][i];
						matrix[row][i] = matrix[switchrow][i];
						matrix[switchrow][i] = tmp;
					}
				}
				else {
					// move column to right and right column to current column
					// This ensures we keep a diagonal on the solution and free values
					// are at the end

					swapcolumn--;
					if (swapcolumn > row) {
						for (int i = 0; i < lightcount; i++) {
							int tmp = matrix[i][row];
							matrix[i][row] = matrix[i][swapcolumn];
							matrix[i][swapcolumn] = tmp;
						}

						// Keep track of swapped columns
						int tmp = swapcolumns[row];
						swapcolumns[row] = swapcolumns[swapcolumn];
						swapcolumns[swapcolumn] = tmp;
					}
				}
			}

			// Only reduce when the diagonal is not zero
			if (matrix[row][row] != 0) {
				for (int row_elim = row + 1 ; row_elim < lightcount; row_elim++) {
					int factor = matrix[row][row];

					// To prevent fractions, make sure the value below the diagonal is a  multiple
					// of the diagonal, scale whole row accordingly
					if (matrix[row_elim][row] % factor) {
						// cannot remove multiples for matrix[...][row]
						//  make sure that all fields are multiples of matrix[row][row]
						for (int i = 0; i < buttoncount + 1; i++) {
							matrix[row_elim][i] *= factor;
						}
					}

					// This factor is alway integer due to previous correction
					int elimfactor = matrix[row_elim][row] / matrix[row][row];

					// Now eliminate on row
					for (int i = 0; i < buttoncount + 1; i++) {
						matrix[row_elim][i] -= matrix[row][i] * elimfactor;
					}
				}
			} 
			else
				rank--;
		}
		

		// We must vary the results for the rows that have rownumber >= rank
		int solution[MAXBUTTON];
		int minpushcount = 1000000;
		memset(solution, 0, sizeof solution);
		do {

			int success = 1;

			// Do back-substitution, taking into account the free parameters
			// Stop back substitution as soon as a single value is either a fraction or negative
			// Rationale: no negative pushes, push is always whole
			for (int button = rank - 1; success && button >= 0; button--) {
				int sum = 0;
				for (int laterbutton = button + 1; laterbutton < buttoncount; laterbutton++) {
					sum += matrix[button][laterbutton] * solution[laterbutton];
				}

				// No fractional solutions!
				if ((matrix[button][buttoncount] - sum) % matrix[button][button])
					success = 0;

				solution[button] = (matrix[button][buttoncount] - sum) / matrix[button][button];

				// Negative solutions are not acceptable
				if (solution[button] < 0) {
					success = 0;
				}

			}


			// Validate if found result matches expected outcome
			for(int light = 0 ; success && light < lightcount; light++) {
				int sum = 0;
				for (int button = 0; button < buttoncount; button++) 
					sum += origmatrix[light][swapcolumns[button]] * solution[button];
				if (sum != origmatrix[light][buttoncount]) {
					success = 0;
				}
			}

			if (success) {
				// Count the pushes
				int pushcount = 0;
				for (int button = 0; button < buttoncount; button++) 
					pushcount += solution[button];
				// if shortest, then save
				if (pushcount < minpushcount) {
					minpushcount = pushcount;
				}
			}

			// Increase free parameters
			nextbutton=buttoncount - 1;
			while (nextbutton >= rank && solution[nextbutton]++ > maxjoltage) {
				solution[nextbutton--] = 0;
			}


		} while (nextbutton >= rank);

		// minpushcount = 10000000 --> No solution found (shouldn't happen!)
		if (minpushcount != 1000000) {
			total += minpushcount;
		}
	}
	printf ("Total pushes: %d\n", total);
	
	return 0;
}
