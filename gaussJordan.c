// This programme performs gauss-jordan elimination for you automatically regardless of the size of your matrix

#include <stdlib.h>
#include <stdio.h>

// Prints the inputed 2d array
void printer (double** array, long rows, long cols) {
	for (long i = 0; i < rows; i += 1) {
		for (long j = 0; j < cols; j += 1) {
			printf("%lf ", array[i][j]);
		}
		printf("\n");
	}
}

//Takes the leading entry and turns it into a 1
void makeOne(double** array, long rows, long cols, long curr_row, long curr_col) {
	double Divisor = array[curr_row][curr_col];
	for (long i = curr_col; i < cols; i += 1) {
		array[curr_row][i] = (double)(array[curr_row][i] / Divisor); 
	}
}

// Turns the entry at row number 'checkRow', column number 'curr_col' into a zero 
void subtract(double** array, long rows, long cols, long curr_row, long curr_col, long checkRow) {
	double multiplier = array[checkRow][curr_col];
	for (long i = curr_col; i < cols; i += 1) {
		array[checkRow][i] = array[checkRow][i] - (multiplier * array[curr_row][i]);
	}
}

// Runs through every entry below the leading entry and if its not a zero inputs its row number into the subtract
// function
void elim(double** array, long rows, long cols, long curr_row, long curr_col) {
	long checkRow = curr_row + 1;
	for (long i = checkRow; i < rows; i += 1) {
		if (array[i][curr_col] != 0) {
			subtract(array, rows, cols, curr_row, curr_col, i);
		}
	}
}

// Cycles through all rows from 'curr_row + 1' to 'rows - 1' to check if there is a non-zero entry
// if there is the row number of the non-zero entry is returned
long nonZeroCheck(double** array, long rows, long curr_row, long curr_col) {
	for (long i = curr_row + 1; i < rows; i += 1) {
		if (array[i][curr_col] != 0) {
			return i;
		}
	}
	return 0;
}

// Swaps the 'curr_row' and 'swapRow'
void swap(double** array, long rows, long cols, long curr_row, long curr_col, long swapRow) {
	double* temp = calloc(cols, sizeof(double));
	temp = array[curr_row];
	array[curr_row] = array[swapRow];
	array[swapRow] = temp;
}

// The gaussean portion of gaussean-jordan elimination, the matrix will be brought down to a row echelon form with
// all leading entries being '1'
//
// The pivotRow and pivotCol arrays takes note of the column number and row number of the leading entry 
void g(double** array, long rows, long cols, long* pivotRow, long* pivotCol) {
	long curr_row = 0;
	long curr_col = 0;
	long i = 0;
	while ((curr_row < rows) && (curr_col < cols)) {
		if (array[curr_row][curr_col] == 1) {
			pivotRow[i] = curr_row + 1;
			pivotCol[i] = curr_col + 1;
			i += 1;
			if ((curr_row + 1 == rows) && (curr_col + 1 == cols)) {
				break;
			}
			elim(array, rows, cols, curr_row, curr_col);
			curr_row += 1;
			curr_col += 1;


		}
		else if (array[curr_row][curr_col] == 0) {
			if (nonZeroCheck(array, rows, curr_row, curr_col) != 0) {
				long swapRow = nonZeroCheck(array, rows, curr_row, curr_col);
				swap(array, rows, cols, curr_row, curr_col, swapRow);
			}
			else {	
				curr_col += 1;
			}
		}
		else {
			makeOne(array, rows, cols, curr_row, curr_col);
		}
	}
}

// The reverse of the elim function from earlier, ie it checks all entries above the leading entries and the row 
// numbers of any non-zero entries are inputed into the subtract function
void reverseElim(double** array, long rows, long cols, long curr_row, long curr_col) {
	long checkRow = curr_row - 1;
	for (long i = checkRow; i >= 0; i -= 1) {
		if (array[i][curr_col] != 0) {
			subtract(array, rows, cols, curr_row, curr_col, i);
		}
	}
}

// The jordan portion of gauss-jordan elimination since we all leading entries are already one and we know the
// exact locations of the leading entries we only need the reverseElim function
void j(double** array, long row, long col, long* pivotRow, long* pivotCol, long maxPiv) {
	for (long i = maxPiv-1; i >= 0; i -= 1) {
		if (pivotRow[i] != 0 && pivotCol[i] != 0) {
			reverseElim(array, row, col, pivotRow[i]-1, pivotCol[i]-1);
		}
	}
}

// Creates an array of size row * col and takes in user input to fill the 2d array as well
double** makeArray(long row, long col) {
	double** array = calloc(row, sizeof(double*));
	if (array == NULL) {
		printf("Memory allocation failed \n");
	}
	for (long i = 0; i < row; i++){
		array[i] = calloc(col, sizeof(double));
		if (array[i] == NULL) {
			printf("Memory allocation failed \n");
			free(array);
		}
	}
	printf("Enter Matrix\n");
	for (long i = 0; i < row; i += 1) {
		for (long j = 0; j < col; j += 1) {
			scanf("%lf", &array[i][j]);
		}
	}
	return array;
}

int main () {
	long row;
	long col;
	long maxPiv;
	printf("Enter row and column: ");
	scanf("%ld %ld", &row,&col);
	double** array = makeArray(row, col);
	if (row >= col) {
		maxPiv = col;
	}
	else {
		maxPiv = row;
	}
	long* pivotRow = calloc(maxPiv, sizeof(long));
	long* pivotCol = calloc(maxPiv, sizeof(long));

	g(array, row, col, pivotRow, pivotCol);
	j(array, row, col, pivotRow, pivotCol, maxPiv);
	printer(array, row, col);
	// Use this if you would like to check the position of the leading entries
	/*for (long i = 0; i < maxPiv; i += 1) {
		printf("Row is %ld , col is %ld \n", pivotRow[i] - 1, pivotCol[i] - 1);
	}*/
	free(pivotCol);
	free(pivotRow);
	for (long i = 0; i < row; i += 1) {
		free(array[i]);
	}
	free(array);	
}
