// Assignment 7, by Leonardo Blas.

#include <iostream>
#include <time.h>
#include <math.h>
#include "FHvector.h"

using namespace std;

// shellSort #1, from FHsort.h.
template <typename Comparable>
void shellSort1(FHvector<Comparable> & a)
{
	int k, pos, arraySize, gap;
	Comparable tmp;
	arraySize = a.size();
	for (gap = arraySize / 2; gap > 0; gap /= 2)
		for (pos = gap; pos < arraySize; pos++)
		{
			tmp = a[pos];
			for (k = pos; k >= gap && tmp < a[k - gap]; k -= gap)
				a[k] = a[k - gap];
			a[k] = tmp;
		}
}

template <typename Comparable>
void shellSortX(FHvector<Comparable> & a, int toSort[], int toSortSize)
{
	int k, pos, arraySize, gap;
	Comparable tmp;
	arraySize = a.size();
	for (gap = toSortSize - 1; gap >= 0; --gap)
		for (pos = toSort[gap]; pos < arraySize; pos++)
		{
			tmp = a[pos];
			for (k = pos;
				k >= toSort[gap] && tmp < a[k - toSort[gap]];
				k -= toSort[gap])
				a[k] = a[k - toSort[gap]];
			a[k] = tmp;
		}
}

int main()
{
	srand(time(NULL));
	clock_t startTime, stopTime;

	FHvector<int> fhVectorOfInts1;
	FHvector<int> fhVectorOfInts2;
	FHvector<int> fhVectorOfInts3;
	FHvector<int> fhVectorOfInts4;
	FHvector<int> fhVectorOfInts5;
	FHvector<int> fhVectorOfInts6;

	// Sizes from from 10,0000 to 200,000.
	cout << "*Sizes:\n";
	for (int i = 0; i < 10000; i++)
		fhVectorOfInts1.push_back(rand());
	cout << "fhVectorOfInts1's size: " << fhVectorOfInts1.size();
	for (int i = 0; i < 45000; i++)
		fhVectorOfInts2.push_back(rand());
	cout << "\nfhVectorOfInts2's size: " << fhVectorOfInts2.size();
	for (int i = 0; i < 91524; i++)
		fhVectorOfInts3.push_back(rand());
	cout << "\nfhVectorOfInts3's size: " << fhVectorOfInts3.size();
	for (int i = 0; i < 122000; i++)
		fhVectorOfInts4.push_back(rand());
	cout << "\nfhVectorOfInts4's size: " << fhVectorOfInts4.size();
	for (int i = 0; i < 199999; i++)
		fhVectorOfInts5.push_back(rand());
	cout << "\nfhVectorOfInts5's size: " << fhVectorOfInts5.size();
	for (int i = 0; i < 200000; i++)
		fhVectorOfInts6.push_back(rand());
	cout << "\nfhVectorOfInts6's size: " << fhVectorOfInts6.size();

	int gapArray[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
		2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
		1048576 };

	const int gapSize = sizeof(gapArray) / sizeof(gapArray[0]);
	int sedgewickArray[gapSize];
	int tokudaArray[gapSize];

	// Sedgewick's gap sequence array (from wikipedia / canvas forums).
	for (int i = 0; i < gapSize; i++)
		sedgewickArray[i] = (i % 2) ?
		(8 * pow(2, i)) - (6 * (pow(2, (i + 1) / 2))) + 1 :
		(9 * (pow(2, i) - pow(2, i / 2))) + 1;

	cout << "\n\n*Sedgewick's Gap Sequence:";
	for (int i = 0; i < gapSize; i++)
	{
		if (i % 7 == 0)
			cout << '\n';
		cout << sedgewickArray[i] << " - ";
	}

	// OEIS says Tokuda's sequence is the best empirical sequence for shell
	// sorting. Ciura's sequence remains unknown.
	for (int i = 0; i < gapSize; i++)
		tokudaArray[i] = ceil(((9. * pow(9./4., i ) - 4 )) / 5.);

	cout << "\n\n*Tokuda's Gap Sequence (my best gap sequence):";
	for (int i = 0; i < gapSize; i++)
	{
		if (i % 7 == 0)
			cout << '\n';
		cout << tokudaArray[i] << " - ";
	}

	// fill distinct vectors with identical random values so we can compare gaps
	// Implicit are already filled, fhVectorOfInts.
	FHvector<int> explicitVector1 = fhVectorOfInts1;
	FHvector<int> explicitVector2 = fhVectorOfInts2;
	FHvector<int> explicitVector3 = fhVectorOfInts3;
	FHvector<int> explicitVector4 = fhVectorOfInts4;
	FHvector<int> explicitVector5 = fhVectorOfInts5;
	FHvector<int> explicitVector6 = fhVectorOfInts6;

	FHvector<int> sedgewickVector1 = fhVectorOfInts1;
	FHvector<int> sedgewickVector2 = fhVectorOfInts2;
	FHvector<int> sedgewickVector3 = fhVectorOfInts3;
	FHvector<int> sedgewickVector4 = fhVectorOfInts4;
	FHvector<int> sedgewickVector5 = fhVectorOfInts5;
	FHvector<int> sedgewickVector6 = fhVectorOfInts6;

	FHvector<int> tokudaVector1 = fhVectorOfInts1;
	FHvector<int> tokudaVector2 = fhVectorOfInts2;
	FHvector<int> tokudaVector3 = fhVectorOfInts3;
	FHvector<int> tokudaVector4 = fhVectorOfInts4;
	FHvector<int> tokudaVector5 = fhVectorOfInts5;
	FHvector<int> tokudaVector6 = fhVectorOfInts6;
	
	// Implied gap sequence.
	cout << "\n\n*Timing:\n";
	cout << "\n*Implied gap sequence:";
	startTime = clock();
	shellSort1(fhVectorOfInts1);
	stopTime = clock();
	cout << "\nimpliedVector1: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSort1(fhVectorOfInts2);
	stopTime = clock();
	cout << "\nimpliedVector2: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSort1(fhVectorOfInts3);
	stopTime = clock();
	cout << "\nimpliedVector3: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSort1(fhVectorOfInts4);
	stopTime = clock();
	cout << "\nimpliedVector4: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSort1(fhVectorOfInts5);
	stopTime = clock();
	cout << "\nimpliedVector5: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSort1(fhVectorOfInts6);
	stopTime = clock();
	cout << "\nimpliedVector6: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;

	// Explicit gap sequence.
	cout << "\n\n*Explicit gap sequence:";
	startTime = clock();
	shellSortX(explicitVector1, gapArray, gapSize);
	stopTime = clock();
	cout << "\nexplicitVector1: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(explicitVector2, gapArray, gapSize);
	stopTime = clock();
	cout << "\nexplicitVector2: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(explicitVector3, gapArray, gapSize);
	stopTime = clock();
	cout << "\nexplicitVector3: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(explicitVector4, gapArray, gapSize);
	stopTime = clock();
	cout << "\nexplicitVector4: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(explicitVector5, gapArray, gapSize);
	stopTime = clock();
	cout << "\nexplicitVector5: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(explicitVector6, gapArray, gapSize);
	stopTime = clock();
	cout << "\nexplicitVector6: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;

	// Sedgewick gap sequence.
	cout << "\n\n*Sedgewick's gap sequence:";
	startTime = clock();
	shellSortX(sedgewickVector1, sedgewickArray, gapSize);
	stopTime = clock();
	cout << "\nsedgewickVector1: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(sedgewickVector2, sedgewickArray, gapSize);
	stopTime = clock();
	cout << "\nsedgewickVector2: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(sedgewickVector3, sedgewickArray, gapSize);
	stopTime = clock();
	cout << "\nsedgewickVector3: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(sedgewickVector4, sedgewickArray, gapSize);
	stopTime = clock();
	cout << "\nsedgewickVector4: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(sedgewickVector5, sedgewickArray, gapSize);
	stopTime = clock();
	cout << "\nsedgewickVector5: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(sedgewickVector6, sedgewickArray, gapSize);
	stopTime = clock();
	cout << "\nsedgewickVector6: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	
	// Tokuda gap sequence.
	cout << "\n\n*Tokuda gap sequence:";
	startTime = clock();
	shellSortX(tokudaVector1, tokudaArray, gapSize);
	stopTime = clock();
	cout << "\ntokudaVector1: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(tokudaVector2, tokudaArray, gapSize);
	stopTime = clock();
	cout << "\ntokudaVector2: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(tokudaVector3, tokudaArray, gapSize);
	stopTime = clock();
	cout << "\ntokudaVector3: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(tokudaVector4, tokudaArray, gapSize);
	stopTime = clock();
	cout << "\ntokudaVector4: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(tokudaVector5, tokudaArray, gapSize);
	stopTime = clock();
	cout << "\ntokudaVector5: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
	startTime = clock();
	shellSortX(tokudaVector6, tokudaArray, gapSize);
	stopTime = clock();
	cout << "\ntokudaVector6: " <<
		(double)(stopTime - startTime) / (double)CLOCKS_PER_SEC;
}

/*--------//--------//--------//--------//--------//--------//--------//--------
SAMPLE RUN:

*Sizes:
fhVectorOfInts1's size: 10000
fhVectorOfInts2's size: 45000
fhVectorOfInts3's size: 91524
fhVectorOfInts4's size: 122000
fhVectorOfInts5's size: 199999
fhVectorOfInts6's size: 200000

*Sedgewick's Gap Sequence:
1 - 5 - 19 - 41 - 109 - 209 - 505 -
929 - 2161 - 3905 - 8929 - 16001 - 36289 - 64769 -
146305 - 260609 - 587521 - 1045505 - 2354689 - 4188161 - 9427969 -

*Tokuda's Gap Sequence (my best gap sequence):
1 - 4 - 9 - 20 - 46 - 103 - 233 -
525 - 1182 - 2660 - 5985 - 13467 - 30301 - 68178 -
153401 - 345152 - 776591 - 1747331 - 3931496 - 8845866 - 19903198 -

*Timing:

*Implied gap sequence:
impliedVector1: 0.02
impliedVector2: 0.136
impliedVector3: 0.289
impliedVector4: 0.369
impliedVector5: 0.671
impliedVector6: 0.799

*Explicit gap sequence:
explicitVector1: 0.065
explicitVector2: 0.321
explicitVector3: 1.27
explicitVector4: 1.792
explicitVector5: 3.009
explicitVector6: 3.743

*Sedgewick's gap sequence:
sedgewickVector1: 0.015
sedgewickVector2: 0.068
sedgewickVector3: 0.14
sedgewickVector4: 0.199
sedgewickVector5: 0.348
sedgewickVector6: 0.361

*Tokuda gap sequence:
tokudaVector1: 0.013
tokudaVector2: 0.085
tokudaVector3: 0.178
tokudaVector4: 0.229
tokudaVector5: 0.337
tokudaVector6: 0.36


ANSWERS TO QUESTIONS:

Why does Shell's gap sequence implied by shellSort1() give a different
timing result than the explicit array described above and passed to
shellSortX()? Which is faster and why?

From what I've learned about effective gap sequences, they are very diverse.
The gap sequence we're using  in our explicit sorting is a simple 2^n 
sequence, meaning it doesn't include odd numbers. Since we're dealing with
randomly generated vectors, which model real life data sets, discriminating
against odd numbers will naturally have a negative effect on our time
complexity. Our implicit sorting is more effective because it's more diverse,
as halving our vectors' size will most likely provide us with odd and even
numbers in our gap sequence, which is more diverse and hence most likely to
be more effective than our explicit sorting.

TABLE:

VECTOR	|	SIZE		|	IMPLICIT	|	EXPLICIT	|	SEDGEWICK	|	TOKUDA
1		10,000			0.02			0.065			0.015			0.013			
2		45,000			0.136			0.321			0.068			0.085
3		91,524			0.289			1.27			0.14			0.178
4		122,000			0.369			1.792			0.199			0.229
5		199,999			0.671			3.009			0.348			0.337
6		200,000			0.799			3.743			0.361			0.36
--------//--------//--------//--------//--------//--------//--------//------*/