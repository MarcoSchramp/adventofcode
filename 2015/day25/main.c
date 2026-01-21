#include <stdio.h>

// From the inputs
long modulo = 33554393; // Prime! --> This allows for modulo calculus
long multiply = 252533; // Prime! --> This allows for modulo calculus

long diagmatrixvalue (long row, long col) {
	return (row + col +1) *(row + col) /2  + col + 1;
}

// Constant calculation time (O(1))
// Smart modulo rekenen
long calcKey(long startValue, long iter) {
	long M = multiply;
	long factor = 1;
	// Max 64 bits, so this is quick solution
	while (iter != 0L) {
		// test for first bit
		if (iter % 2L != 0L) 
			factor = (factor * M) % modulo;
		M = (M * M) % modulo;
		// shift left one bit
		iter = iter / 2L ;
	}
	return (startValue * factor) % modulo;
}

int main(int argc, char* argv) {

	// From the inputs
	long row = 2947;
	long column = 3029;
	long startvalue = 20151125;

	// the formule kan met modulo-rekenen teruggebracht worden tot 33554393 punten (nog steeds veel, maar doenbaar!)

	// minus, because diagmatrixvalue uses zero-based indexes
	long iter = diagmatrixvalue(row - 1, column - 1) - 1;

	// The sequence is based on modulo calculus, to it repeats itself after modulo steps! 
	// Removing all unnecessary repeats
	iter = iter % modulo;

	// Or even if we did, calc-key is also O(1) , so we don't need to..
	printf("Key = %ld", calcKey(startvalue, iter));
	return 0;

}
