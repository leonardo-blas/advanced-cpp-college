//// Assignment 5, by Leonardo Blas.
//
//#ifndef FHSPLAYTREE_H
//#define FHSPLAYTREE_H
//#include <iostream>
//#include "FHsearch_tree.h"
//#include "FHavlTree.h"
//using namespace std;
//
//template <class Comparable>
//class FHsplayTree : public FHsearch_tree < Comparable >
//{
//public:
//	bool insert(const Comparable &x);
//	bool remove(const Comparable &x);
//	bool contains(const Comparable &x);
//	const Comparable & find(const Comparable &x);
//	const Comparable & showRoot();
//protected:
//	void splay(FHs_treeNode<Comparable> *&root, const Comparable &x);
//	void rotateWithLeftChild(FHs_treeNode<Comparable> *&k2);
//	void rotateWithRightChild(FHs_treeNode<Comparable> *&k2);
//	typedef FHs_treeNode<Comparable> Node;
//	typedef typename FHsearch_tree<Comparable>::EmptyTreeException EmptyTree;
//	typedef typename FHsearch_tree<Comparable>::NotFoundException NotFound;
//};
//
//template <typename Object>
//class PrintObject
//{
//public:
//	void operator()(Object obj)
//	{
//		cout << obj << " ";
//	}
//};
//
//////MAIN 1, FROM THE DISCUSSIONS FORUM
////int main()
////{
////	int k, k2;
////	FHsplayTree<int> searchTree;
////	//FHsearch_tree<int> searchTree;
////	PrintObject<int> intPrinter;
////
////	searchTree.traverse(intPrinter);
////	//cout << searchTree.showRoot() << endl;
////
////	cout << "Initial size: " << searchTree.size() << endl;
////	for (k = 1; k <= 32; k++)
////		searchTree.insert(k);
////	cout << "New size: " << searchTree.size() << endl;
////
////	cout << "\nTraversal: \n";
////	searchTree.traverse(intPrinter);
////	cout << endl << endl;
////
////	for (k = -1; k < 13; k++)
////	{
////		// test contains()
////		k2 = 10 - k;
////		if (!searchTree.contains(k2))
////			cout << " oops on contains " << k2 << endl;
////		cout << "contains " << k2 << " --> root: " << searchTree.showRoot()
////			<< " height: " << searchTree.showHeight() << endl;
////
////		// test find()
////		try
////		{
////			searchTree.find(k);
////		}
////		catch (...)
////		{
////			cout << " oops on find " << k << endl;
////		}
////		cout << "find " << k << " --> root: " << searchTree.showRoot()
////			<< " height: " << searchTree.showHeight() << endl;
////	}
////
////	return 0;
////}
//
//
//// MAIN 2, FROM THE ASSIGNMENTS PAGE
//int main()
//{
//	int k;
//	FHsplayTree<int> searchTree;
//	//FHsearch_tree<int> searchTree;
//	PrintObject<int> intPrinter;
//
//	searchTree.traverse(intPrinter);
//	//cout << searchTree.showRoot() << endl;
//
//	cout << "Initial size: " << searchTree.size() << endl;
//	for (k = 1; k <= 32; k++)
//		searchTree.insert(k);
//	cout << "New size: " << searchTree.size() << endl;
//
//	cout << "\nTraversal: \n";
//	searchTree.traverse(intPrinter);
//	cout << endl << endl;
//
//	for (k = -1; k < 10; k++)
//	{
//		// searchTree.contains(k);  // alt to find() - different error return
//		try
//		{
//			searchTree.find(k);
//		}
//		catch (...)
//		{
//			cout << " oops ";
//		}
//		cout << "splay " << k << " --> root: " << searchTree.showRoot()
//			<< " height: " << searchTree.showHeight() << endl;
//	}
//
//	return 0;
//}
//
//template <class Comparable>
//bool FHsplayTree<Comparable>::insert(const Comparable &x)
//{
//	// Code repetition in purpose, trying to avoid chunking and future problems,
//	// yet following the algorithm's architecture.
//	if (!this->mRoot)
//	{
//		this->mRoot = new Node(x, NULL, NULL);
//		this->mSize++;
//		return true;
//	}
//	splay(this->mRoot, x);
//	if (x < this->mRoot->data)
//	{
//		this->mRoot = new Node(x, this->mRoot->lftChild, this->mRoot);
//		this->mSize++;
//		return true;
//	}
//	// CANNOT BE else if (x > this->mRoot->data)
//	else if (this->mRoot->data < x)
//	{
//		this->mRoot = new Node(x, this->mRoot, this->mRoot->rtChild);
//		this->mSize++;
//		return true;
//	}
//	return false;
//}
//
//template <class Comparable>
//bool FHsplayTree<Comparable>::remove(const Comparable &x)
//{
//	if (!this->mRoot)
//		return false;
//	splay(this->mRoot, x);
//	// CANNOT BE if (x != this->mRoot->data)
//	if (this->mRoot->data < x || x < this->mRoot->data)
//		return false;
//	Node *newRoot;
//	if (!this->mRoot->lftChild)
//		newRoot = this->mRoot->rtChild;
//	else
//	{
//		newRoot = this->mRoot->lftChild;
//		splay(newRoot, x);
//		newRoot->rtChild = this->mRoot->rtChild;
//	}
//	delete this->mRoot;
//	this->mRoot = newRoot;
//	this->mSize--;
//	return true;
//}
//
//template <class Comparable>
//bool FHsplayTree<Comparable>::contains(const Comparable &x)
//{
//	try
//	{
//		find(x);
//	}
//	catch (EmptyTree exception)
//	{
//		return false;
//	}
//	catch (NotFound exception)
//	{
//		return false;
//	}
//	return true;
//}
//
//template <class Comparable>
//const Comparable & FHsplayTree<Comparable>::find(const Comparable &x)
//{
//	if (!this->mRoot)
//		throw EmptyTree();
//	splay(this->mRoot, x);
//	// CANNOT BE (x == this->mRoot)
//	if (!(this->mRoot->data < x || x < this->mRoot->data))
//		return this->mRoot->data;
//	throw NotFound();
//}
//
//template <class Comparable>
//const Comparable & FHsplayTree<Comparable>::showRoot()
//{
//	return !this->mRoot ? Comparable() : this->mRoot->data;
//}
//
//template <class Comparable>
//void FHsplayTree<Comparable>::splay(FHs_treeNode<Comparable>
//	*&root,
//	const Comparable &x)
//{
//	// right and leftTree MUST NOT be actual trees, but nodes.
//	Node *rightTree = NULL, *leftTree = NULL;
//	Node *rightTreeMin = NULL, *leftTreeMax = NULL;
//	while (root)
//	{
//		if (x < root->data)
//		{
//			if (!root->lftChild)
//				break;
//			if (x < root->lftChild->data)
//			{
//				// Zig zig.
//				rotateWithLeftChild(root);
//				if (!root->lftChild)
//					break;
//			}
//			// Check if our "tree" is empty.
//			if (!rightTreeMin)
//				// Conceptual "addition" of root to rightTree's minimum node.
//				rightTreeMin = rightTree = root;
//			// If not empty, we continue to lftChild, as x < root->data.
//			else
//				rightTreeMin = rightTreeMin->lftChild = root;
//			// "update the new working root: set root to root's left child"
//			root = root->lftChild;
//		}
//		// Same process.
//		// CANNOT BE if (x > root->data)
//		else if (root->data < x)
//		{
//			if (!root->rtChild)
//				break;
//			if (root->rtChild->data < x)
//			{
//				// Zig zig.
//				rotateWithRightChild(root);
//				if (!root->rtChild)
//					break;
//			}
//			// Check if our "tree" is empty.
//			if (!leftTreeMax)
//				// Conceptual "addition" of root to rightTree's minimum node.
//				leftTreeMax = leftTree = root;
//			// If not empty, we continue to rtChild, as x > root->data.
//			else
//				leftTreeMax = leftTreeMax->rtChild = root;
//			// "update the new working root: set root to root's left child"
//			root = root->rtChild;
//		}
//		// Without the else statement it will always break.
//		else
//			break;
//	}
//	if (leftTree)
//	{
//		// leftTreeMax->rtChild = leftTree's maximum.
//		leftTreeMax->rtChild = root->lftChild;
//		root->lftChild = leftTree;
//	}
//	if (rightTree)
//	{
//		// rightTreeMin->lftChild = rightTree's minimum.
//		rightTreeMin->lftChild = root->rtChild;
//		root->rtChild = rightTree;
//	}
//}
//
//template <class Comparable>
//void FHsplayTree<Comparable>::rotateWithLeftChild(
//	FHs_treeNode<Comparable> *&k2)
//{
//	// Very similar to FHavlTree.h's, but height is not needed.
//	Node *k1 = k2->lftChild;
//	k2->lftChild = k1->rtChild;
//	k1->rtChild = k2;
//	k2 = k1;
//}
//
//template <class Comparable>
//void FHsplayTree<Comparable>::rotateWithRightChild(
//	FHs_treeNode<Comparable> *&k2)
//{
//	// Very similar to FHavlTree.h's, but height is not needed.
//	Node *k1 = k2->rtChild;
//	k2->rtChild = k1->lftChild;
//	k1->lftChild = k2;
//	k2 = k1;
//}
//#endif
//
///*--------//--------//--------//--------//--------//--------//--------//--------
//SAMPLE RUNS:
////--------//--------//--------//--------//--------//--------//--------//--------
//MAIN 1, FROM THE DISCUSSIONS FORUM
//
//Initial size: 0
//New size: 32
//
//Traversal:
//1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
//
//contains 11 --> root: 11 height: 11
// oops on find -1
//find -1 --> root: 1 height: 13
//contains 10 --> root: 10 height: 12
// oops on find 0
//find 0 --> root: 1 height: 13
//contains 9 --> root: 9 height: 13
//find 1 --> root: 1 height: 14
//contains 8 --> root: 8 height: 14
//find 2 --> root: 2 height: 15
//contains 7 --> root: 7 height: 15
//find 3 --> root: 3 height: 16
//contains 6 --> root: 6 height: 16
//find 4 --> root: 4 height: 17
//contains 5 --> root: 5 height: 17
//find 5 --> root: 5 height: 17
//contains 4 --> root: 4 height: 18
//find 6 --> root: 6 height: 16
//contains 3 --> root: 3 height: 18
//find 7 --> root: 7 height: 15
//contains 2 --> root: 2 height: 17
//find 8 --> root: 8 height: 14
//contains 1 --> root: 1 height: 16
//find 9 --> root: 9 height: 13
// oops on contains 0
//contains 0 --> root: 1 height: 15
//find 10 --> root: 10 height: 12
// oops on contains -1
//contains -1 --> root: 1 height: 14
//find 11 --> root: 11 height: 11
// oops on contains -2
//contains -2 --> root: 1 height: 13
//find 12 --> root: 12 height: 7
//
//
////--------//--------//--------//--------//--------//--------//--------//--------
//// MAIN 2, FROM THE ASSIGNMENTS PAGE
//
//Initial size: 0
//New size: 32
//
//Traversal:
//1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32
//
// oops splay -1 --> root: 1 height: 16
// oops splay 0 --> root: 1 height: 16
//splay 1 --> root: 1 height: 16
//splay 2 --> root: 2 height: 9
//splay 3 --> root: 3 height: 6
//splay 4 --> root: 4 height: 6
//splay 5 --> root: 5 height: 5
//splay 6 --> root: 6 height: 6
//splay 7 --> root: 7 height: 6
//splay 8 --> root: 8 height: 7
//splay 9 --> root: 9 height: 8
//
////--------//--------//--------//--------//--------//--------//--------//------*/