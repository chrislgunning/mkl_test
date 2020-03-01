#include <chrono>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "mkl.h"

using namespace std;

enum INDEX_MAP { INDEX_COLS = 1, INDEX_ROWS };

const int MIN_NUM_ARGUMENTS = 3;

void mkl_cumsum(int num_cols, int num_rows, double *input, double *output);

void gnu_cumsum(int num_cols, int num_rows, double *input, double *output);

int main(int argc, char **argv) {

	if(argc < MIN_NUM_ARGUMENTS){
		cout << "E: Usage - './mkl_test <number of columns> <number of rows>'" << endl;
		return 1;
	}

	const int NUM_COLS = atoi(argv[INDEX_COLS]);
	const int NUM_ROWS = atoi(argv[INDEX_ROWS]);

	double input[NUM_COLS];
	double output[NUM_COLS];

	srand(time(0));

	for (int iDat = 0; iDat < NUM_COLS; iDat++)
	{
		input[iDat] = (double)rand();
	}

	auto time_start = chrono::high_resolution_clock::now();
	mkl_cumsum(NUM_COLS, NUM_ROWS, &input[0], &output[0]);
	auto time_end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(time_end - time_start).count();
	cout << "MKL Time: " << duration << endl;


	time_start = chrono::high_resolution_clock::now();
	gnu_cumsum(NUM_COLS, NUM_ROWS, &input[0], &output[0]);
	time_end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(time_end - time_start).count();
	cout << "GNU Time: " << duration << endl;

	return 0;
}

void mkl_cumsum(int num_cols, int num_rows, double *input, double *output) {
	for (int iRow = 0; iRow < num_rows; iRow++)
	{
		for (int iCol = 0; iCol < num_cols; iCol++) {
			output[iCol] = input[iCol] + 1.0e-27;
		}
		vdLog10(num_cols, &output[0], &output[0]);
		cblas_dscal(num_cols, 10.0, &output[0], 1);
		vdAdd(num_cols, &input[0], &output[0], &output[0]);
	}
}

void gnu_cumsum(int num_cols, int num_rows, double *input, double *output) {
	for (int iRow = 0; iRow < num_rows; iRow++)
	{
		for (int iCol = 0; iCol < num_cols; iCol++) {
			output[iCol] = log10(input[iCol] + 1.0e-27);
			output[iCol] *= 10.0;
			output[iCol] += input[iCol];
		}
	}
}
