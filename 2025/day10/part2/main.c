#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The problem statement can be formulated as a linear problem
// that can be solved using guassian elimination.
// The solution can be under specifield (rank < length of diagonal)
// The solution can also have full rank, but additional "buttons (columns)
// which allow for different solutions.

// After gaussian elemination, the program runs through possible
// combinations on the "free" buttons, resolving the "determined" buttons
// and validates the resulting number of button presses

// Maximum length of a line (1000 is enough!)
#define MAXLINE 1000
// Maximum of 30 buttons
#define MAXBUTTON 30

// Maximum of 30 lights
#define MAXLIGHT 30

int main(int argc, char *argv[])
{
	// Line buffer for reading
	char line[MAXLINE];

	// Total counter
	int total = 0;

	while (fgets(line, sizeof line, stdin) != NULL) {
		// This matrix must be loaded and gaussian eliminated
		int matrix[MAXBUTTON][MAXLIGHT];

		// This is a backup, matrix used during validation
		int origmatrix[MAXBUTTON][MAXLIGHT];

		// We do column swaps to manage rank issues
		int swapcolumns[MAXBUTTON];

		// The number of lights in the problem
		int lightcount = 0;

		// The number of buttons in the problem
		int buttoncount = 0;

		// Clear matrix with zeros
		memset(matrix, 0, sizeof matrix);
		int maxjoltage = 0;

		// Parsing input --> space is used to separate blocks
		char *strtok_space = NULL;
		for (char *part = strtok_r(line, " ", &strtok_space);
		     part ; part = strtok_r(NULL, " ", &strtok_space)) {
			// Block with lights status (previous assignment)
			// Now only used to count the lights
			if (*part == '[') {
				part++;
				while (*(part++) != ']') {
					lightcount++;
				}
			}
					
			// Block with button to light configuration
			// Each block is a new button
			// Each column in the matrix represents a button
			// Each cell indicates if this adds to a light (0 or 1)
			if (*part == '(') {
				char *strtok_idx = NULL;
				for(char *idx = strtok_r(part + 1, ",)", &strtok_idx);
				    idx; idx = strtok_r(NULL, ",)", &strtok_idx)) {
					int x = atoi(idx);
					// Connect button to light
					matrix[x][buttoncount] = 1;
				}
				swapcolumns[buttoncount] = buttoncount;
				buttoncount++;
			}

			// Block with targets
			// Targets are added as an additional column
			// at the end of the matrix
			if (*part == '{') {
				// Add column to the right
				char *strtok_idx = NULL;
				int light = 0;
				for(char *idx = strtok_r(part + 1, ",}", &strtok_idx);
				    idx; idx = strtok_r(NULL, ",}", &strtok_idx)) {
					int requiredjoltage = atoi(idx);
					matrix[light][buttoncount] = requiredjoltage;
					// Detect the maximum joltage needed
					if (requiredjoltage > maxjoltage) maxjoltage = requiredjoltage;
					light++;
				}
			}
		}

		// Now the matrix is setup and we know its dimensions

		// Keep a copy for validation later
		memcpy(origmatrix, matrix, sizeof matrix);

		// do gaussian elimination

		// We sometimes swap columns in order to keep non-zero values on the diagonal.
		// Columns will which cause zero on diagonal will be moved to the right
		// This happen more than once. swapcolumn keeps track which right-column to swap
		int swapcolumn = buttoncount;

		// 
		int rank = lightcount;

		// Eliminate: start at top row, going down
		for (int row =0; row < lightcount; row++) {

			// zero found on diagonal --> row or column swap needed. 
			while (row < swapcolumn && row < lightcount && matrix[row][row] == 0)  {

				// Find a row below without 0 in the same column
				int switchrow;
				for(switchrow = row + 1; switchrow < lightcount; switchrow++) {
					if (matrix[switchrow][row] != 0) break;
				}

				// Only swap if a row with non-zero is found
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

					// Only swap if this column has not been swapped before
					if (swapcolumn > row) {
						// Swap column
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
			
				// Iterate through rows below
				for (int row_elim = row + 1 ; row_elim < lightcount; row_elim++) {
					int factor = matrix[row][row];

					// To prevent fractions, make sure the value below the diagonal is a  multiple
					// of the diagonal, scale whole row accordingly
					if (matrix[row_elim][row] % factor) {
						// cannot remove multiples for matrix[...][row]
						//  make sure that all fields are multiples of matrix[row][row]
						for (int i = 0; i < buttoncount + 1; i++) 
							matrix[row_elim][i] *= factor;
					}

					// This factor is alway integer due to previous correction
					int elimfactor = matrix[row_elim][row] / matrix[row][row];

					// Now eliminate on row
					for (int i = 0; i < buttoncount + 1; i++)
						matrix[row_elim][i] -= matrix[row][i] * elimfactor;
				}
			} 
			else 
				// Diagonal position with 0 --> reduce rank
				rank--;
		}
		

		// We must vary the results for the rows that have rownumber >= rank
		int solution[MAXBUTTON];

		// Assume the number of button pushes is less than 1000000
		int minpushcount = 1000000;

		// Next button is used to manage the "free" buttons. It indicates which button
		// was increased in the iteration. If this nextbutton < rank, this indicates that
		// the increase would raise a non-free parameter --> We're done
		int nextbutton=buttoncount;

		// Initialize all free-buttons with value 0
		memset(solution, 0, sizeof solution);

		while (nextbutton >= rank) {
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


			// Validate if found result matches expected outcome (only when backsubstitution was valid)
			for(int light = 0 ; success && light < lightcount; light++) {
				int sum = 0;
				for (int button = 0; button < buttoncount; button++) 
					sum += origmatrix[light][swapcolumns[button]] * solution[button];
				if (sum != origmatrix[light][buttoncount])
					success = 0;
			}

			// Yay, a valid solution, let's count the number of button pushes
			// When less than minimum, we have a winner
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
		}


		// minpushcount = 10000000 --> No solution found (shouldn't happen!)
		if (minpushcount != 1000000) {
			total += minpushcount;
		}
	}
	printf ("Total pushes: %d\n", total);
	
	return 0;
}
