//Assignment 3, by Leonardo Blas.

#include <iostream>
#include <iomanip>
#include <random>
#include "FHsparseMat.h"

#define MAT_SIZE 900
typedef float* DynMat[MAT_SIZE];

using namespace std;

template <class Object>
class SparseMatWMult : public SparseMat<double>
{
public:
	SparseMatWMult(int numRows, int numCols);
	bool matMult(SparseMatWMult<Object> & a, SparseMatWMult<Object> & b);
};

typedef SparseMatWMult<float> SpMat;

int main() 
{
	clock_t startTime, stopTime;
	double randFrac;
	int randRow, randCol, smallPercent;

	SpMat inputMatrix(MAT_SIZE, MAT_SIZE);
	SpMat outputMatrix(MAT_SIZE, MAT_SIZE);

	uniform_int_distribution<int> randomInt(0, MAT_SIZE - 1);
	uniform_real_distribution<double> randomFrac(0., 1.);
	default_random_engine re;
	// generate small% (bet .1 and 10%) non-default values (bet 0 and 1)
	smallPercent = MAT_SIZE/1000. * MAT_SIZE;  // div by 20. means 5%, of course
	for (int r = 0; r < smallPercent; r++)
	{
		 randRow = randomInt(re);
		 randCol = randomInt(re);
		 randFrac = randomFrac(re);
		 inputMatrix.set(randRow, randCol, randFrac);
	}

	// 10x10 submatrix in lower right
	inputMatrix.showSubSquare(MAT_SIZE - 10, 10);

	startTime = clock(); // ------------------ start
	outputMatrix.matMult(inputMatrix, inputMatrix);
	stopTime = clock(); // ---------------------- stop

	outputMatrix.showSubSquare(MAT_SIZE - 10, 10);
	cout << "\nSize = " << MAT_SIZE << " Dyn Array Mult Elapsed Time: "
		<< (double)(stopTime - startTime) / (double)CLOCKS_PER_SEC
		<< " seconds." << endl << endl;

	cout << endl;
}

template <class Object>
SparseMatWMult<Object>::SparseMatWMult(int numRows, int numCols)
	: SparseMat(numRows, numCols, 0.0) { }

template <class Object>
bool SparseMatWMult<Object>::matMult(
	SparseMatWMult<Object> & a,
	SparseMatWMult<Object> & b)
{
	// Dimension compatibility and 1x1 size checking.
	if (a.colSize != b.rowSize
		|| a.rowSize < 1 || a.colSize < 1
		|| b.rowSize < 1 || b.colSize < 1)
		return false;
	RowIterator aRowEnd;
	RowIterator bRowEnd;
	double currDotProduct;
	for (int i = 0; i < a.colSize; i++)
	{
		// Checking for empty rows in A.
		if (a.rows[i].size() == 0)
			continue;
		// Checking for empty columns in B would be optimal, but our
		// system's architecture complicates this.
		for (int j = 0; j < b.rowSize; j++)
		{
			currDotProduct = 0;
			aRowEnd = a.rows[i].end();
			for (RowIterator aRowIter = a.rows[i].begin();
				aRowIter != aRowEnd;
				aRowIter++)
			{
				bRowEnd = b.rows[(*aRowIter).getCol()].end();
				// bColIter will be a RowIterator but really intend to check on
				// B columns.
				for (RowIterator bColIter = b.rows[(*aRowIter).getCol()].begin();
					bColIter != bRowEnd;
					bColIter++)
					// We check for every element in every individual B's column.
					if ((*bColIter).getCol() == j)
						currDotProduct += ((*aRowIter).data) * ((*bColIter).data);
						break;
			}
			if (currDotProduct != 0)
				set(i, j, currDotProduct);
		}
	}
	return true;
}

/*--------//--------//--------//--------//--------//--------//--------//--------
SAMPLE RUNS AND ANSWERS:

//--------//--------//--------//--------//--------//--------//--------//--------
PART A:

SOME SAMPLE RUNS:

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

//--------//--------//--------//--------//--------//--------//--------//--------
PART B:

SOME SAMPLE RUNS:

5% SPARSE SQUARE MATRICES:

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

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.06 0.00 0.13
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 150 Dyn Array Mult Elapsed Time: 0.02 seconds.



Sub-Matrix of size 10x10, starting at position (290, 290) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.99 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.59 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.78
 0.00 0.00 0.00 0.00 0.00 0.76 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (290, 290) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.20 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 300 Dyn Array Mult Elapsed Time: 0.14 seconds.



Sub-Matrix of size 10x10, starting at position (440, 440) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.07 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.19 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.59 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (440, 440) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.06 0.00 0.00 0.00
 0.00 0.00 0.16 0.00 0.00 0.00 0.00 0.00 0.03 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.15 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.03 0.00 0.00 0.00 0.00 0.00 0.01 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.58 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 450 Dyn Array Mult Elapsed Time: 0.43 seconds.


Sub-Matrix of size 10x10, starting at position (590, 590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.02 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (590, 590) :

 0.00 0.00 0.00 0.00 0.00 0.06 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.58 0.00 0.00 0.00 0.00 0.00 0.17 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.20 0.00 0.00 0.00 0.00 0.21 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 600 Dyn Array Mult Elapsed Time: 0.99 seconds.


Sub-Matrix of size 10x10, starting at position (740, 740) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.52 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.04 0.90 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.18
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.27 0.00

Sub-Matrix of size 10x10, starting at position (740, 740) :

 0.00 0.00 0.31 0.00 0.00 0.00 0.21 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.21 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 750 Dyn Array Mult Elapsed Time: 1.86 seconds.



Sub-Matrix of size 10x10, starting at position (890, 890) :

 0.00 0.00 0.00 0.24 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.07 0.00 0.88 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.56 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.74 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (890, 890) :

 0.00 0.00 0.00 0.00 0.00 0.06 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.15 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.11
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.04
 0.00 0.00 0.01 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.18 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 900 Dyn Array Mult Elapsed Time: 3.20 seconds.



1. Are the times longer or shorter than for dynamic matrices?
		--------
		|Part A|
		--------	
		M		t
		150	0.1
		300	0.11
		450	0.39
		600	0.98
		750	2.09
		900	3.50
	
	The times for dynamic matrices are generally longer, until the M = 750
	point is reached.

2. Are the growth rates larger or smaller?  By how much?
	Shorter. After plotting and carrying on a graphic recursive analysis, we
	learn that this function grows at a cubic rate.

	The difference between Part A and Part B:

	R^2 for cubic regression = 0.9996 (Part A)
	R^2 for cubic regression = 1 (Part B)

	We could argue that our Part B is truly a theta(n^3) cubic growth function,
	while Part A only reaches O(n^3).

3. Create a table and answer the same questions as before.
		M		t
		150	0.02
		300	0.14
		450	0.43
		600	0.99
		750	1.86
		900	3.20

4. What was the largest M you could use here, and was the reason the same or
	different than for dynamic arrays?
	Considering we have 3 loops, and although their iterations vary depending
	on several factors, because we are dealing with square matrices, our number
	of loop iterations (in all loops) will be roughly the same. This is the logic
	I used to estimate Part B would be modeled, at least, by O(n^3).
	My results confirm my hypothesis, and furthermore show the fact that we have a
	theta(n^3) growth function.
	The largest size I tried was M = 900 for all %'s. Again, my computer can 
	probably handle more but waiting is not time efficient.

5. If you have time, modify 1% to be .5% or .1% or even less and see if the 
	growth rates change.

	---------------
	|REQUIRED RUNS:|
	----------------

1% SPARSE SQUARE MATRICES:

Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
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

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 800 Dyn Array Mult Elapsed Time: 1.66 seconds.


Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.02 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 1600 Dyn Array Mult Elapsed Time: 4.19 seconds.


Sub-Matrix of size 10x10, starting at position (3190, 3190) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.59 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (3190, 3190) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 3200 Dyn Array Mult Elapsed Time: 28.52 seconds.


0.5% SPARSE SQUARE MATRICES:

Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 800 Dyn Array Mult Elapsed Time: 0.46 seconds.


Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 1600 Dyn Array Mult Elapsed Time: 2.59 seconds.


Sub-Matrix of size 10x10, starting at position (3190, 3190) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.59 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (3190, 3190) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.07 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 3200 Dyn Array Mult Elapsed Time: 16.36 seconds.


0.5% SPARSE SQUARE MATRICES:

Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (790, 790) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 800 Dyn Array Mult Elapsed Time: 0.16 seconds.



Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (1590, 1590) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 1600 Dyn Array Mult Elapsed Time: 1.13 seconds.



Sub-Matrix of size 10x10, starting at position (3190, 3190) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00

Sub-Matrix of size 10x10, starting at position (3190, 3190) :

 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00
 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00


Size = 3200 Dyn Array Mult Elapsed Time: 6.33 seconds.


INVESTIGATING IF THE GROWTH RATE CHANGES WITH THE %:

1% SPARSE SQUARE MATRICES:
		M		t
		150	0.01
		300	0.06
		450	0.17
		600	0.32
		750	0.56
		900	1.01

R^2 for cubic regression = 0.999
O(n^3)

0.5% SPARSE SQUARE MATRICES:
		150	0.01
		300	0.04
		450	0.11
		600	0.22
		750	0.38
		900	0.59
R^2 for cubic regression = 1
O(n^3), theta(n^3)

0.5% SPARSE SQUARE MATRICES:
		M		t
		150	0.00
		300	0.01
		450	0.03
		600	0.20
		750	0.42
		900	0.64
R^2 for cubic regression = 0.9953
O(n^3)

Conclussion:
No, changing the % does not alter the growth function.
//--------//--------//--------//--------//--------//--------//--------//------*/