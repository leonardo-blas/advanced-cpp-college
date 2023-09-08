//Assignment 3, by Leonardo Blas.

#include <iostream>
#include <random>
#include <iomanip>

#define MAT_SIZE 500
typedef float* DynMat[MAT_SIZE];

using namespace std;

void matShowDyn(const DynMat & matA, int start, int size);
void matMultDyn(const DynMat & matA, const DynMat & matB, DynMat & matC, int size);

int main()
{
  int r, c;
  clock_t startTime, stopTime;
  double randFrac;
  int randRow, randCol, smallPercent;

	// non-sparse dynamic matrix
	DynMat matDyn, matDynAns;

	uniform_int_distribution<int> randomInt(0, MAT_SIZE - 1);
	uniform_real_distribution<double> randomFrac(0., 1.);
	default_random_engine re;
	// allocate rows and initialize to 0
	for (r = 0; r < MAT_SIZE; r++)
	{
		matDyn[r] = new float[MAT_SIZE];
		matDynAns[r] = new float[MAT_SIZE];
		for (c = 0; c < MAT_SIZE; c++)
		{
			matDyn[r][c] = 0.;
			matDynAns[r][c] = 0.;
		}
	}

	// generate small% (bet .1 and 10%) non-default values (bet 0 and 1)
	smallPercent = MAT_SIZE / 20. * MAT_SIZE;  // div by 20. means 5%
	for (int r = 0; r < smallPercent; r++)
	{
		randRow = randomInt(re);
		randCol = randomInt(re);
		randFrac = randomFrac(re);
		matDyn[randRow][randCol] = randFrac;
	}

	// 10x10 submatrix in lower right
	matShowDyn(matDyn, MAT_SIZE - 10, 10);

	startTime = clock();  // ------------------ start
	matMultDyn(matDyn, matDyn, matDynAns, MAT_SIZE);
	stopTime = clock();  // ---------------------- stop

	matShowDyn(matDynAns, MAT_SIZE - 10, 10);
	cout << "\nSize = " << MAT_SIZE << " Dyn Array Mult Elapsed Time: "
		<< (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC
		<< " seconds." << endl << endl;

	// clean up
	for (r = 0; r < MAT_SIZE; r++)
	{
		delete[] matDyn[r];
		delete[] matDynAns[r];
	}
	cout << endl;
}

void matMultDyn(
	const DynMat & matA,
	const DynMat & matB,
	DynMat & matC,
	int size) 
{
	// i is for iterating through A's rows.
	// j is for iterating through A's columns.
	// k is for iterating through B's rows.
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
		{
			matC[i][j] = 0;
			for (int k = 0; k < size; k++)
				// The Dot Product is very specific. AB is not equal to BA.
				matC[i][j] += matA[i][k] * matB[k][j];
		}
}

void matShowDyn(const DynMat & matA, int start, int size)
{
	int subMatrixSize = start + size;
	if (start < 0 || size < 1 || MAT_SIZE < subMatrixSize)
	{
		cout << "Not possible to display a matrix of size "
			<< size << "x" << size << ", starting at position ("
			<< start << ", " << start << ").\n\n";
		return;
	}
	cout << "Sub-Matrix of size " << size << "x" << size
		<< ", starting at position (" << start << ", "
		<< start << ") :\n\n";
	// Simple 2-D loop.
	for (int i = start; i < subMatrixSize; i++)
	{
		for (int j = start; j < subMatrixSize; j++)
			cout << fixed << setprecision(2) << setw(5) << matA[i][j];
		cout << '\n';
	}
	cout << '\n';
}

/*--------//--------//--------//--------//--------//--------//--------//--------
SAMPLE RUNS AND ANSWERS:

Sub-Matrix of size 10x10, starting at position (990, 990) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.74 0.68 0.00 0.00
 0.02 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.86 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (990, 990) :

 1.13 1.05 0.67 0.55 0.80 0.25 1.63 0.41 0.38 0.22
 0.00 0.00 0.91 0.24 0.83 0.13 0.28 0.07 1.08 0.43
 1.01 0.32 1.51 0.92 0.27 1.04 0.15 0.50 0.42 0.64
 0.39 0.19 1.46 0.00 1.13 0.22 0.04 1.10 0.16 0.01
 0.52 1.30 0.99 0.00 0.30 0.89 0.56 1.35 0.66 0.99
 0.00 0.81 1.52 0.79 0.03 0.32 0.47 0.00 0.56 0.52
 0.11 0.52 0.00 0.12 0.67 0.42 0.64 0.54 0.01 0.43
 1.57 1.16 0.00 1.18 1.49 1.57 0.12 0.42 0.53 0.09
 1.00 0.13 1.28 0.56 0.89 1.04 0.35 0.52 1.73 0.80
 0.81 0.52 1.47 0.32 0.30 0.37 0.48 1.16 0.00 0.00


Size = 1000 Dyn Array Mult Elapsed Time: 6.56 seconds.



Sub-Matrix of size 10x10, starting at position (140, 140) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.56 0.00
 0.95 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.33 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.45 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.44 0.00 0.00 0.00 0.00 0.00 0.18
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (140, 140) :

 0.00 0.00 0.08 0.00 0.30 0.09 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.24 0.00 0.00 0.00 0.00 0.00 0.10
 0.12 0.11 0.00 0.00 0.00 0.00 0.10 0.00 0.00 0.00
 0.18 0.00 0.41 0.57 0.04 0.00 0.00 0.51 0.00 0.00
 0.42 0.40 0.17 0.00 0.00 0.00 1.13 0.00 0.15 0.00
 0.00 0.00 0.10 0.00 0.00 0.14 0.00 0.06 0.00 0.13
 0.00 0.00 0.00 0.20 0.00 0.00 0.00 0.26 0.00 0.08
 0.00 0.00 0.15 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.03 0.00 0.00 0.00 0.63 0.00 0.01
 0.00 0.00 0.16 0.43 0.51 0.12 0.00 0.51 0.00 0.00


Size = 150 Dyn Array Mult Elapsed Time: 0.01 seconds.



Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.59 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.84 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.59

Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.42 0.09 1.02 0.65 2.15 0.98 0.47 0.70 0.33 0.19
 1.57 0.10 0.18 0.16 0.22 0.46 0.80 0.23 1.94 1.85
 0.29 1.56 1.03 0.44 0.69 1.02 0.89 2.96 0.57 1.28
 1.79 1.14 1.36 1.68 1.18 0.77 0.83 0.96 0.15 1.30
 1.17 1.21 0.57 0.56 1.25 1.29 0.55 0.55 1.17 0.51
 0.00 0.81 0.76 1.47 0.37 0.99 1.10 0.79 0.88 1.15
 0.47 1.25 0.50 0.94 0.29 1.11 1.33 1.30 1.75 2.42
 1.33 1.68 0.12 1.96 1.31 0.37 1.47 0.43 1.69 2.34
 0.21 0.65 3.03 0.83 1.14 0.85 1.39 0.95 1.16 2.04
 0.83 0.85 0.16 0.27 0.28 0.12 0.62 1.33 0.52 0.60


Size = 1600 Dyn Array Mult Elapsed Time: 31.89 seconds.



Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.00 0.00 0.00 0.00 0.04 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.44 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.14 0.30 0.28 0.54 0.26 0.88 0.41 0.44 0.71 1.11
 0.24 0.01 0.07 0.16 0.07 0.00 0.00 0.19 0.14 0.76
 1.25 0.18 0.59 0.72 0.61 0.42 0.62 0.74 0.00 0.86
 0.17 0.28 0.06 1.13 0.13 0.25 0.78 0.07 0.00 0.04
 0.06 0.33 0.02 0.01 0.46 0.21 0.22 0.05 0.00 0.59
 0.04 0.03 0.09 0.00 0.00 0.58 0.06 0.48 0.04 0.25
 0.35 0.81 0.00 0.09 0.51 0.94 0.67 0.15 0.81 0.69
 0.26 0.00 0.09 0.00 0.23 0.04 0.29 0.17 0.02 0.00
 0.39 0.77 0.31 0.54 0.64 0.21 0.85 0.67 1.71 0.59
 0.23 0.76 0.58 0.32 0.09 0.15 0.00 1.11 1.44 0.24


Size = 800 Dyn Array Mult Elapsed Time: 2.52 seconds.



My Estimate  for O() and theta():
	Because we have a triple loop in MatMultDyn(), all of which depend on size,
	I believe the O() and theta() will follow a cubic growth function:
	O(n^3), theta(n^3).

1. What was the smallest M that gave you a non-zero time?
	125, 0.01 seconds.

2. What happened when you doubled M , tripled it, quadrupled it, etc?  
	Give several M values and their times in a table.
		M		t
		150	0.1
		300	0.11
		450	0.39
		600	0.98
		750	2.09
		900	3.50

3. How large an M can you use before the program refuses to run (exception or 
	run-time error due to memory overload) or it takes so long you can't wait	
	for the run?
	M = 1600 took 31.89 seconds.
	My computer can probably handle a larger M, but the waiting is too long.

4. How did the data agree or disagree with your original time complexity 
	estimate?
	My hypothesis and my data are in line. I plotted my data points and they
	follow a cubic polynomial trend. 
	R^2 for cubic regression = 0.9996
	R^2 for quadratic regression = 0.9992
	R^2 for 4th degree regression = 0.9892
//--------//--------//--------//--------//--------//--------//--------//------*/