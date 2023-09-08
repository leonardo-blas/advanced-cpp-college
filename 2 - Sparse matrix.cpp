/*--------//--------//--------//--------//--------//--------//--------//--------
Assignment 2, by Leonardo Blas.
General comment: The most challenging part of this assignment was to get used
to using user defined headers, rather than the libraries that C++ provides us
with, such as <vector>. Although both ADTs, the ones C++ provides us with and
the ones given to us by our instructor, are pretty similar, they are not
exactly the same.
/*--------//--------//--------//--------//--------//--------//--------//------*/
#include <iostream>
#include <iomanip>
#include "FHlist.h"
#include "FHvector.h"

#define MAT_SIZE 100000

using namespace std;

template <class Object>
class MatNode
{
protected:
	int col;

public:
	Object data;
	// we need a default constructor for lists
	MatNode(int cl = 0, Object dt = Object()) : col(cl), data(dt) {}
	int getCol() const { return col; }

	// not optimized yet for set() = defaultVal;  refer to forums
	const Object & operator=(const Object &x) { return (data = x); }
};

template <class Object>
class SparseMat
{
protected:
	typedef FHlist< MatNode<Object> > MatRow;
	typedef FHvector<MatRow> MasterCol;
	typedef typename MatRow::iterator RowIterator;
	typedef typename MatRow::const_iterator RowConstIterator;
	typedef typename MasterCol::iterator ColIterator;
	typedef typename MasterCol::const_iterator ColConstIterator;
	MasterCol rows;
	Object defaultVal;
	int rowSize, colSize;
public:
	SparseMat(int r, int c, const Object & defaultVal);
	const Object & get(int r, int c) const;
	bool set(int r, int c, const Object &x);
	// EXTRA CREDIT METHOD.
	const Object & set(int r, int c);
	void clear();
	void showSubSquare(int start, int size);
	class OffLimitsException { };
};

template <class Object>
SparseMat<Object>::SparseMat(
	int r,
	int c,
	const Object &defaultVal)
{
	rows = (r >= 1) ? MasterCol(r) : MasterCol();
	rowSize = rows.size();
	colSize = (c >= 1) ? c : 0;
	this->defaultVal = defaultVal;
}

template <class Object>
const Object & SparseMat<Object>::get(int r, int c) const
{
	if (r < 0 || c < 0 || rowSize <= r || colSize <= c)
		throw OffLimitsException();
	RowConstIterator rowsEnd = rows[r].end();
	// Searching algorithm.
	for (RowConstIterator iter = rows[r].begin();
		iter != rowsEnd; iter++) // ++ operator visits next node.
		// -> operator not overloaded in header file.
		if ((*iter).getCol() == c)
			return (*iter).data;
	return defaultVal;
}

template <class Object>
bool SparseMat<Object>::set(int r, int c, const Object &x)
{
	if (r < 0 || c < 0 || rowSize <= r || colSize <= c)
		return false;
	RowIterator iter;
	RowIterator rowsEnd = rows[r].end();
	// Searching and setting algorithm.
	for (iter = rows[r].begin();
		iter != rowsEnd && (*iter).getCol() <= c;
		iter++)
		if ((*iter).getCol() == c)
		{
			if (x != defaultVal)
				(*iter).data = x;
			else
				rows[r].erase(iter);
			return true;
		}
	// If not found, x is inserted into the row.
	if (x != defaultVal)
		rows[r].insert(iter, MatNode<Object>(c, x));
	return true;
}

// EXTRA CREDIT METHOD.
template <class Object>
const Object & SparseMat<Object>::set(int r, int c)
{
	Object x = Object();
	//if (this->set(r, c, x));
	return x;
}

//const SparseMat & operator=(const Object &rhs)


template <class Object>
void SparseMat<Object>::clear()
{
	for (int i = 0; i < rowSize; i++)
		rows[i].clear();
}

template <class Object>
void SparseMat<Object>::showSubSquare(int start, int size)
{
	int subMatrixSize = start + size;
	// From the forums: "... please check the boundary condition
	// and ignore those positions outside the matrix."
	if (rowSize < subMatrixSize || colSize < subMatrixSize
		|| start < 0 || size < 1) {
		cout << "Not possible to display a matrix of size "
			<< size << "x" << size << ", starting at position ("
			<< start << ", " << start << ").\n\n";
		return;
	}
	cout << "Sub-Matrix of size " << size << "x" << size
		<< ", starting at position (" << start << ", "
		<< start << ") :\n\n";
	for (int i = start; i < start + size; i++)
	{
		for (int j = start; j < start + size; j++)
			cout << setw(4) << get(i, j);
		cout << '\n';
	}
	cout << '\n';
}

typedef SparseMat<float> SpMat;

// --------------- main ---------------
int main()
{
  SpMat mat(MAT_SIZE, MAT_SIZE, 0); // 100000 x 100000 filled with 0

  // test mutators
  mat.set(2, 5, 10);
  mat.set(2, 5, 35);   // should overwrite the 10
  mat.set(3, 9, 21);
  mat.set(MAT_SIZE, 1, 5);  // should fail silently
  mat.set(9, 9, mat.get(3, 9));  // should copy the 21 here
  mat.set(4,4, -9);
  mat.set(4,4, 0);   // should remove the -9 node entirely
  mat.set(MAT_SIZE-1, MAT_SIZE-1, 99);

  // test accessors and exceptions
  try
  {
     cout << mat.get(7, 8) << endl;
     cout << mat.get(2, 5) << endl;
     cout << mat.get(9, 9) << endl;
     cout << mat.get(-4, 7) << endl;  // should throw an exception
  }
  catch (...)
  {
     cout << "oops" << endl;
  }

  // show top left 15x15
  mat.showSubSquare(0, 15);

  // show bottom right 15x15
  mat.showSubSquare(MAT_SIZE - 15, 15);

	mat.showSubSquare(0, 0);
	mat.showSubSquare(0, -1);
	mat.showSubSquare(-1, 0);
	mat.showSubSquare(-1, -1);

	SpMat mat2(10, 10, 0);
	mat2.showSubSquare(5, 5);
	mat2.set(0, 0, 13);
	mat2.set(0, -1, 14);
	mat2.set(-1, 0, 14);
	mat2.set(-1, -1, 15);
	mat2.showSubSquare(0,10);
	mat2.set(0, 0, 0);
	mat2.showSubSquare(0, 10);
	mat2.set(0, 0, 444);
	mat2.set(5, 5, 555);
	mat2.set(9, 9, 777);
	mat2.showSubSquare(0, 10);
	mat2.clear();
	mat2.showSubSquare(0, 10);
}

/*--------//--------//--------//--------//--------//--------//--------//--------
SAMPLE RUN:

0
35
21
oops
Sub-Matrix of size 15x15, starting at position (0, 0) :

	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0  35   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0  21   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0  21   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0

Sub-Matrix of size 15x15, starting at position (99985, 99985) :

	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0   0   0   0   0  99

Not possible to display a matrix of size 0x0, starting at position (0, 0).

Not possible to display a matrix of size -1x-1, starting at position (0, 0).

Not possible to display a matrix of size 0x0, starting at position (-1, -1).

Not possible to display a matrix of size -1x-1, starting at position (-1, -1).

Sub-Matrix of size 5x5, starting at position (5, 5) :

	0   0   0   0   0
	0   0   0   0   0
	0   0   0   0   0
	0   0   0   0   0
	0   0   0   0   0

Sub-Matrix of size 10x10, starting at position (0, 0) :

 13   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0

Sub-Matrix of size 10x10, starting at position (0, 0) :

	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0

Sub-Matrix of size 10x10, starting at position (0, 0) :

444   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0 555   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0 777

Sub-Matrix of size 10x10, starting at position (0, 0) :

	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0
	0   0   0   0   0   0   0   0   0   0

//--------//--------//--------//--------//--------//--------//--------//------*/
