//#include <iostream>
//#include <iomanip>
//#include "FHlist.h"
//#include "FHvector.h"
//#include "MatNode.h"
//using namespace std;
//
//template <class Object>
//class SparseMat
//{
//protected:
//	typedef FHlist< MatNode<Object> > MatRow;
//	typedef FHvector<MatRow> MasterCol;
//	typedef typename MatRow::iterator RowIterator;
//	typedef typename MatRow::const_iterator RowConstIterator;
//	typedef typename MasterCol::iterator ColIterator;
//	typedef typename MasterCol::const_iterator ColConstIterator;
//	MasterCol rows;
//	Object defaultVal;
//	int rowSize, colSize;
//public:
//	SparseMat(int r, int c, const Object & defaultVal);
//	const Object & get(int r, int c) const;
//	bool set(int r, int c, const Object &x);
//	// EXTRA CREDIT METHOD.
//	const Object & set(int r, int c);
//	void clear();
//	void showSubSquare(int start, int size);
//	class OffLimitsException { };
//};
//
//template <class Object>
//SparseMat<Object>::SparseMat(
//	int r,
//	int c,
//	const Object &defaultVal)
//{
//	rows = (r >= 1) ? MasterCol(r) : MasterCol();
//	rowSize = rows.size();
//	colSize = (c >= 1) ? c : 0;
//	this->defaultVal = defaultVal;
//}
//
//template <class Object>
//const Object & SparseMat<Object>::get(int r, int c) const
//{
//	if (r < 0 || c < 0 || rowSize <= r || colSize <= c)
//		throw OffLimitsException();
//	RowConstIterator rowsEnd = rows[r].end();
//	// Searching algorithm.
//	for (RowConstIterator iter = rows[r].begin();
//		iter != rowsEnd; iter++) // ++ operator visits next node.
//		// -> operator not overloaded in header file.
//		if ((*iter).getCol() == c)
//			return (*iter).data;
//	return defaultVal;
//}
//
//template <class Object>
//bool SparseMat<Object>::set(int r, int c, const Object &x)
//{
//	if (r < 0 || c < 0 || rowSize <= r || colSize <= c)
//		return false;
//	RowIterator iter;
//	RowIterator rowsEnd = rows[r].end();
//	// Searching and setting algorithm.
//	for (iter = rows[r].begin();
//		iter != rowsEnd && (*iter).getCol() <= c;
//		iter++)
//		if ((*iter).getCol() == c)
//		{
//			if (x != defaultVal)
//				(*iter).data = x;
//			else
//				rows[r].erase(iter);
//			return true;
//		}
//	// If not found, x is inserted into the row.
//	if (x != defaultVal)
//		rows[r].insert(iter, MatNode<Object>(c, x));
//	return true;
//}
//
//// EXTRA CREDIT METHOD.
//template <class Object>
//const Object & SparseMat<Object>::set(int r, int c)
//{
//	Object x = Object();
//	//if (this->set(r, c, x));
//	return x;
//}
//
////const SparseMat & operator=(const Object &rhs)
//
//
//template <class Object>
//void SparseMat<Object>::clear()
//{
//	for (int i = 0; i < rowSize; i++)
//		rows[i].clear();
//}
//
//template <class Object>
//void SparseMat<Object>::showSubSquare(int start, int size)
//{
//	int subMatrixSize = start + size;
//	// From the forums: "... please check the boundary condition
//	// and ignore those positions outside the matrix."
//	if (rowSize < subMatrixSize || colSize < subMatrixSize
//		|| start < 0 || size < 1) {
//		cout << "Not possible to display a matrix of size "
//			<< size << "x" << size << ", starting at position ("
//			<< start << ", " << start << ").\n\n";
//		return;
//	}
//	cout << "Sub-Matrix of size " << size << "x" << size
//		<< ", starting at position (" << start << ", "
//		<< start << ") :\n\n";
//	for (int i = start; i < start + size; i++)
//	{
//		for (int j = start; j < start + size; j++)
//			cout << fixed << setprecision(2) << setw(5) << get(i, j);
//		cout << '\n';
//	}
//	cout << '\n';
//}