// Assignment 4, by Leonardo Blas.

#ifndef FHLAZYSEARCHTREE_H
#define FHLAZYSEARCHTREE_H

#include <iostream>
#include <string>
#include <stack>

using namespace std;

template <typename Object>
class PrintObject
{
public:
	void operator()(Object obj)
	{
		cout << obj << " ";
	}
};

// ---------------------- FHlazySearchTreeNode Prototype -----------------------
template <class Comparable>
class FHlazySearchTreeNode
{
public:
	// Addition of deleted to the init list.
	FHlazySearchTreeNode(const Comparable & d = Comparable(),
		FHlazySearchTreeNode *lt = NULL,
		FHlazySearchTreeNode *rt = NULL,
		bool dltd = false)
		: lftChild(lt), rtChild(rt), data(d), deleted(dltd)
	{ }
	FHlazySearchTreeNode *lftChild, *rtChild;
	Comparable data;
	// New deleted member.
	bool deleted;
	// for use only with AVL Trees
	virtual int getHeight() const { return 0; }
	virtual bool setHeight(int height) { return true; }
};

// ---------------------- FHlazySearchTree Prototype --------------------------
template <class Comparable>
class FHlazySearchTree
{
protected:
	// New mSizeHard member.
	int mSize, mSizeHard;
	FHlazySearchTreeNode<Comparable> *mRoot;

public:
	FHlazySearchTree() { mSize = 0; mRoot = NULL; }
	FHlazySearchTree(const FHlazySearchTree &rhs)
	{
		mRoot = NULL; mSize = 0; *this = rhs;
	}
	~FHlazySearchTree() { clear(); }
	const Comparable &findMin() const;
	const Comparable &findMax() const;
	const Comparable &find(const Comparable &x) const;
	bool empty() const { return (mSize == 0); }
	int size() const { return mSize; }
	void clear() { makeEmpty(mRoot); }
	const FHlazySearchTree & operator=(const FHlazySearchTree &rhs);
	bool insert(const Comparable &x);
	bool remove(const Comparable &x);
	bool contains(const Comparable &x) const { return find(mRoot, x) != NULL; }
	template <class Processor>
	void traverse(Processor func) const { traverse(mRoot, func); }
	int showHeight() const { return findHeight(mRoot); }
	// Public, wrapper collectGarbage().
	void collectGarbage() { collectGarbage(mRoot); }
	// Accessor for mSizeHard.
	int sizeHard() const { return mSizeHard; }

protected:
	FHlazySearchTreeNode<Comparable> *clone(
		FHlazySearchTreeNode<Comparable> *root) const;
	FHlazySearchTreeNode<Comparable> *findMin(
		FHlazySearchTreeNode<Comparable> *root) const;
	FHlazySearchTreeNode<Comparable> *findMax(
		FHlazySearchTreeNode<Comparable> *root) const;
	FHlazySearchTreeNode<Comparable> *find(
		FHlazySearchTreeNode<Comparable> *root,
		const Comparable &x) const;
	bool insert(FHlazySearchTreeNode<Comparable> * &root,
		const Comparable &x);
	bool remove(FHlazySearchTreeNode<Comparable> * &root, const Comparable &x);
	void makeEmpty(FHlazySearchTreeNode<Comparable> * &subtreeToDelete);
	template <class Processor>
	void traverse(FHlazySearchTreeNode<Comparable> *treeNode,
		Processor func, int level = -1) const;
	int findHeight(
		FHlazySearchTreeNode<Comparable> *treeNode, int height = -1) const;
	// Modified remove(), removeHard().
	bool removeHard(FHlazySearchTreeNode<Comparable> * &root,
		const Comparable &x);
	FHlazySearchTreeNode<Comparable> *findHardMin(
		FHlazySearchTreeNode<Comparable> *root) const;

private:
	// Private, recursive collectGarbage().
	void collectGarbage(FHlazySearchTreeNode<Comparable> * &root);

public:
	// for exception throwing
	class EmptyTreeException {};
	class NotFoundException {};
};

int main()
{
	int k;
	const int NUMBER_OF_ITEMS = 10;
	FHlazySearchTree<int> searchTree;
	PrintObject<int> intPrinter;

	searchTree.traverse(intPrinter);

	cout << "INITIALIZED TREE 1, A FHLAZYSEARCHTREE OBJECT.\n";
	cout << "initial size: " << searchTree.size() << endl;
	// Trying findMin() and findMax() in a hard empty tree case.
	cout << "CALLING FINDMIN() AND FINDMAX() IN A HARD EMPTY TREE.\n";
	try
	{
		cout << "findMin() = " << searchTree.findMin() << '\n';
		cout << "findMax() = " << searchTree.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	cout << "\n\n";
	for (int i = 0; i < NUMBER_OF_ITEMS; i++)
	{
		k = (i % 2) ? (i * 10) : (i * -10);
		cout << "INSERTING: " << k << '\n';
		searchTree.insert(k);
	}

	// Testing find().
	cout << "\n\nTESTING FIND IN TREE 1\n";
	cout << "TRYING TO FIND 500\n";
	try
	{
		searchTree.find(500);
		cout << "NO EXCEPTION TRIGGERED, 500 WAS FOUND";
	}
	catch (FHlazySearchTree<int>::NotFoundException exception)
	{
		cout << "Not Found Exception triggered\n";
	}

	cout << "\n\nINSERTED " << NUMBER_OF_ITEMS << " ITEMS.\n";
	cout << "TREE 1 -- traversal and sizes: \n";
	searchTree.traverse(intPrinter);
	cout << "\ntree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;
	cout << "Collecting garbage on tree 1 - should be no garbage." << endl;
	searchTree.collectGarbage();
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	// Trying findMin() and findMax() in a hard full tree case.
	cout << "\n\nTRYING FINDMIN() AND FINDMAX() IN A FULL TREE, TREE 1.\n";
	cout << "CALLING FINDMIN() AND FINDMAX().\n";
	try
	{
		cout << "findMin() = " << searchTree.findMin() << '\n';
		cout << "findMax() = " << searchTree.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	// test assignment operator
	cout << "\n\nTESTING THE ASSIGNMENT OPERATOR\n";
	cout << "INITIALIZED TREE 2, A FHLAZYTREE OBJECT\n";
	FHlazySearchTree<int> searchTree2 = searchTree;
	cout << "ASSIGNED TREE 1 TO TREE 2\n";
	cout << "TREE 2 -- traversal and sizes: \n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;
	cout << "Collecting garbage on tree 2 - should be no garbage." << endl;
	searchTree2.collectGarbage();
	cout << "tree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	// Trying to delete a non-existing node in tree 1.
	cout << "\n\nDELETING A NON-EXISTING ITEM IN TREE 1.\n";
	cout << "TRYING TO REMOVE " << k + 1 << '\n';
	searchTree.remove(k + 1);
	cout << "TREE 1 -- traversal and sizes: \n";
	searchTree.traverse(intPrinter);
	cout << "\ntree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;
	cout << "Collecting garbage on tree 1 - should be no garbage." << endl;
	searchTree.collectGarbage();
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	// Soft deleting the entire tree 1.
	cout << "\n\nSOFT DELETING THE ENTIRE TREE 1.\n";
	for (int i = 0; i < NUMBER_OF_ITEMS; i++)
	{
		k = (i % 2) ? (i * 10) : (i * -10);
		cout << "SOFT DELETING: " << k << '\n';
		searchTree.remove(k);
	}
	cout << "TREE 1 -- traversal and sizes: \n";
	searchTree.traverse(intPrinter);
	cout << "\ntree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	// Trying findMin() and findMax() in a full soft deleted tree case.
	cout << "\n\nTRYING FINDMIN() AND FINDMAX() IN A SOFT DELETED"
		<< " TREE, TREE 1.\n";
	cout << "CALLING FINDMIN() AND FINDMAX().\n";
	try
	{
		cout << "findMin() = " << searchTree.findMin() << '\n';
		cout << "findMax() = " << searchTree.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	// Garbage collection on fully soft deleted tree 1.
	cout << "\n\nCOLLECTGARBAGE() ON THE FULLY SOFT DELETED TREE 1\n.";
	cout << "Collecting garbage on tree 1 - should be no garbage." << endl;
	searchTree.collectGarbage();
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	// Removal on empty tree.
	cout << "\n\nATTEMPTING A REMOVAL ON EMPTY TREE 1: \n";
	if (searchTree.remove(20))
		cout << "removed " << 20 << endl;
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;
	cout << "Collecting Garbage - no change expected. " << endl;
	searchTree.collectGarbage();
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	// test soft insertion and deletion:
	cout << "\n\nAdding 'hard' 22 - should see new sizes. " << endl;
	searchTree.insert(22);
	searchTree.traverse(intPrinter);
	cout << "\ntree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	cout << "\n\nSOFT REMOVING 22.\n";
	searchTree.remove(22);
	searchTree.traverse(intPrinter);
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard();

	cout << "\n\n\nRepeating soft removal. Should see no change. " << endl;
	searchTree.remove(22);
	searchTree.traverse(intPrinter);
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	cout << "\n\nSoft insertion. Hard size should not change. " << endl;
	searchTree.insert(22);
	searchTree.traverse(intPrinter);
	cout << "\ntree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	cout << "\n\nAttempting 100 removals: \n";
	for (int k = 100; k > 0; k--)
	{
		if (searchTree.remove(k))
			cout << "removed " << k << endl;
	}
	searchTree.collectGarbage();

	cout << "\n\nsearchTree now:\n";
	searchTree.traverse(intPrinter);
	cout << "tree 1 size: " << searchTree.size()
		<< "  Hard size: " << searchTree.sizeHard() << endl;

	cout << "\n\nsearchTree2 now:\n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	// Inserting the same items into tree 2.
	cout << "\n\nINSERTING THE SAME ITEMS INTO TREE 2.\n";
	for (int i = 0; i < NUMBER_OF_ITEMS; i++)
	{
		k = (i % 2) ? (i * 10) : (i * -10);
		cout << "INSERTING: " << k << '\n';
		searchTree2.insert(k);
	}

	cout << "\n\nsearchTree2 now:\n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	// Inserting different items into tree 2.
	cout << "\n\nINSERTING THE DIFFERENT ITEMS INTO TREE 2.\n";
	for (int i = 0; i < NUMBER_OF_ITEMS; i++)
	{
		k = (i % 2) ? (i * 10) + 5 : (i * -10) - 5;
		cout << "INSERTING: " << k << '\n';
		searchTree2.insert(k);
	}

	cout << "\n\nsearchTree2 now:\n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	cout << "\n\nTRYING FINDMIN() AND FINDMAX() IN A FULL"
		<< " TREE, TREE 2.\n";
	cout << "CALLING FINDMIN() AND FINDMAX().\n";
	try
	{
		cout << "findMin() = " << searchTree2.findMin() << '\n';
		cout << "findMax() = " << searchTree2.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	cout << "\n\nSOFT DELETING SOME ITEMS IN TREE 2:\n";
	for (int i = 0; i < NUMBER_OF_ITEMS; i++)
	{
		k = (i % 2) ? (i * 10) : (i * -10);
		cout << "SOFT DELETING: " << k << '\n';
		searchTree2.remove(k);
	}


	cout << "\n\nsearchTree2 now:\n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	// Testing find().
	cout << "\n\nTESTING FIND IN TREE 2\n";
	cout << "TRYING TO FIND -5\n";
	try
	{
		searchTree2.find(-5);
		cout << "NO EXCEPTION TRIGGERED, -5 WAS FOUND";
	}
	catch (FHlazySearchTree<int>::NotFoundException exception)
	{
		cout << "Not Found Exception triggered\n";
	}


	cout << "\n\nTRYING FINDMIN() AND FINDMAX() IN A PARTIAL"
		<< " TREE, TREE 2.\n";
	cout << "CALLING FINDMIN() AND FINDMAX().\n";
	try
	{
		cout << "findMin() = " << searchTree2.findMin() << '\n';
		cout << "findMax() = " << searchTree2.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	cout << "\n\nASSIGNING NEW MIN AND MAX TO TREE 2\n";
	cout << "INSERTING: " << -500 << '\n';
	searchTree2.insert(-500);
	cout << "INSERTING: " << 500 << '\n';
	searchTree2.insert(500);

	cout << "\n\nsearchTree2 now:\n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	cout << "\n\nTRYING FINDMIN() AND FINDMAX() IN A PARTIAL"
		<< " TREE, TREE 2.\n";
	cout << "CALLING FINDMIN() AND FINDMAX().\n";
	try
	{
		cout << "findMin() = " << searchTree2.findMin() << '\n';
		cout << "findMax() = " << searchTree2.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	// Testing find().
	cout << "\n\nTESTING FIND IN TREE 2\n";
	cout << "TRYING TO FIND 500\n";
	try
	{
		searchTree2.find(500);
		cout << "NO EXCEPTION TRIGGERED, 500 WAS FOUND";
	}
	catch (FHlazySearchTree<int>::NotFoundException exception)
	{
		cout << "Not Found Exception triggered\n";
	}

	// Testing find().
	cout << "\n\nTESTING FIND IN TREE 2\n";
	cout << "TRYING TO FIND 555\n";
	try
	{
		searchTree2.find(555);
		cout << "NO EXCEPTION TRIGGERED, 555 WAS FOUND";
	}
	catch (FHlazySearchTree<int>::NotFoundException exception)
	{
		cout << "Not Found Exception triggered";
	}

	cout << "\n\n\nCOLLECTING GARBAGE IN TREE 2.\n";
	searchTree2.collectGarbage();

	cout << "\n\nsearchTree2 now:\n";
	searchTree2.traverse(intPrinter);
	cout << "\ntree 2 size: " << searchTree2.size()
		<< "  Hard size: " << searchTree2.sizeHard() << endl;

	cout << "\n\nTRYING FINDMIN() AND FINDMAX() IN A PARTIAL"
		<< " TREE, TREE 2.\n";
	cout << "CALLING FINDMIN() AND FINDMAX().\n";
	try
	{
		cout << "findMin() = " << searchTree2.findMin() << '\n';
		cout << "findMax() = " << searchTree2.findMax() << '\n';
	}
	catch (FHlazySearchTree<int>::EmptyTreeException exception)
	{
		cout << "EmptyTreeException triggered.\n";
	}

	return 0;
}

// FHlazySearchTree public method definitions -----------------------------
template <class Comparable>
const Comparable & FHlazySearchTree<Comparable>::findMin() const
{
	if (mRoot == NULL)
		throw EmptyTreeException();
	return findMin(mRoot)->data;
}

template <class Comparable>
const Comparable & FHlazySearchTree<Comparable>::findMax() const
{
	if (mRoot == NULL)
		throw EmptyTreeException();
	return findMax(mRoot)->data;
}

template <class Comparable>
const Comparable &FHlazySearchTree<Comparable>::find(
	const Comparable &x) const
{
	FHlazySearchTreeNode<Comparable> *resultNode;
	resultNode = find(mRoot, x);
	if (resultNode == NULL)
		throw NotFoundException();
	return resultNode->data;
}

template <class Comparable>
const FHlazySearchTree<Comparable> &FHlazySearchTree<Comparable>::operator=
(const FHlazySearchTree &rhs)
{
	if (&rhs != this)
	{
		clear();
		mRoot = clone(rhs.mRoot);
		mSize = rhs.size();
		mSizeHard = rhs.sizeHard();
	}
	return *this;
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::insert(const Comparable &x)
{
	if (insert(mRoot, x))
	{
		mSize++;
		return true;
	}
	return false;
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::remove(const Comparable &x)
{
	if (remove(mRoot, x))
	{
		mSize--;
		return true;
	}
	return false;
}

template <class Comparable>
template <class Processor>
void FHlazySearchTree<Comparable>::traverse(
	FHlazySearchTreeNode<Comparable> *treeNode,
	Processor func, int level) const
{
	if (treeNode == NULL)
		return;
	// we're not doing anything with level but its there in case we want it
	traverse(treeNode->lftChild, func, level + 1);
	// We check for deletion.
	if (!treeNode->deleted)
		func(treeNode->data);
	traverse(treeNode->rtChild, func, level + 1);
}

// FHlazySearchTree private method definitions -----------------------------
template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::clone(
	FHlazySearchTreeNode<Comparable> *root) const
{
	FHlazySearchTreeNode<Comparable> *newNode;
	if (root == NULL)
		return NULL;
	// We also clone the deleted attribute.
	newNode = new FHlazySearchTreeNode<Comparable>(
		root->data,
		clone(root->lftChild),
		clone(root->rtChild),
		root->deleted);
	return newNode;
}

template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findMin(
	FHlazySearchTreeNode<Comparable> *root) const
{
	if (root == NULL)
		return NULL;
	// Local variables belong to their scope, so we can create temp variables!
	FHlazySearchTreeNode<Comparable> *lastNotDeleted = NULL;
	// lastNotDeleted points to the last not deleted node traversed.
	if (!root->deleted)
		lastNotDeleted = root;
	if (root->lftChild == NULL)
	{
		if (lastNotDeleted == NULL)
		{// Pointing at a new default node so we dont get any runtime errors.
			lastNotDeleted = new FHlazySearchTreeNode<Comparable>;
		}
		return lastNotDeleted;
	}
	return findMin(root->lftChild);
}

template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findMax(
	FHlazySearchTreeNode<Comparable> *root) const
{
	if (root == NULL)
		return NULL;
	// Local variables belong to their scope, so we can create temp variables!
	FHlazySearchTreeNode<Comparable> *lastNotDeleted = NULL;
	// lastNotDeleted points to the last not deleted node traversed.
	if (!root->deleted)
		lastNotDeleted = root;
	if (root->rtChild == NULL)
	{
		if (lastNotDeleted == NULL)
			// Pointing at a new default node so we dont get any runtime errors.
			lastNotDeleted = new FHlazySearchTreeNode<Comparable>;
		return lastNotDeleted;
	}
	return findMax(root->rtChild);
}

template <class Comparable>
FHlazySearchTreeNode<Comparable>* FHlazySearchTree<Comparable>::find(
	FHlazySearchTreeNode<Comparable> *root, const Comparable &x) const
{
	// We also check if the root is deleted.
	if (root == NULL)
		return NULL;
	if (x < root->data)
		return find(root->lftChild, x);
	if (root->data < x)
		return find(root->rtChild, x);
	(!root->deleted) ? root : NULL;
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::insert(
	FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
{
	if (root == NULL)
	{
		root = new FHlazySearchTreeNode<Comparable>(x, NULL, NULL);
		++mSizeHard;
		return true;
	}
	else if (x < root->data)
		return insert(root->lftChild, x);
	else if (root->data < x)
		return insert(root->rtChild, x);
	// If the root is soft deleted we just un-delete it.
	else if (root->deleted)
	{
		root->deleted = false;
		return true;
	}
	return false; // duplicate
}

template <class Comparable>
bool FHlazySearchTree<Comparable>::remove(
	FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
{
	if (root == NULL)
		return false;
	if (x < root->data)
		return remove(root->lftChild, x);
	if (root->data < x)
		return remove(root->rtChild, x);
	// If not deleted, mark it for deletion. DON'T hard delete it.
	if (!root->deleted)
	{
		root->deleted = true;
		return true;
	}
	return false;
}

template <class Comparable>
void FHlazySearchTree<Comparable>::makeEmpty(
	FHlazySearchTreeNode<Comparable> * &subtreeToDelete)
{
	if (subtreeToDelete == NULL)
		return;
	// remove children
	makeEmpty(subtreeToDelete->lftChild);
	makeEmpty(subtreeToDelete->rtChild);
	// clear client's pointer
	delete subtreeToDelete;
	subtreeToDelete = NULL;
	// New size conditions.
	if (!subtreeToDelete->deleted)
		--mSize;
	--mSizeHard;
}

template <class Comparable>
int FHlazySearchTree<Comparable>::findHeight(
	FHlazySearchTreeNode<Comparable> *treeNode, int height) const
{
	int leftHeight, rightHeight;
	if (treeNode == NULL)
		return height;
	height++;
	leftHeight = findHeight(treeNode->lftChild, height);
	rightHeight = findHeight(treeNode->rtChild, height);
	return (leftHeight > rightHeight) ? leftHeight : rightHeight;
}

template <class Comparable>
void FHlazySearchTree<Comparable>::collectGarbage(
	FHlazySearchTreeNode<Comparable> * &root)
{
	// We use the same algorithm as in makeEmpty(), as it traverses
	// the entire tree and has similar purposes.
	if (root == NULL)
		return;
	// remove children
	collectGarbage(root->lftChild);
	collectGarbage(root->rtChild);
	// We simply cast the already defined removeHard() method.
	if (root->deleted)
	{
		removeHard(root, root->data);
		--mSizeHard;
	}
}

// removeHard() is just a modified version of the original remove().
template <class Comparable>
bool FHlazySearchTree<Comparable>::removeHard(
	FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
{
	if (root == NULL)
		return false;
	if (x < root->data)
		return removeHard(root->lftChild, x);
	if (root->data < x)
		return removeHard(root->rtChild, x);
	// found the node
	if (root->lftChild != NULL && root->rtChild != NULL)
	{
		// Use the original findMin(), but with a different name.
		FHlazySearchTreeNode<Comparable> *minNode = findHardMin(root->rtChild);
		root->data = minNode->data;
		// Update member info.
		root->deleted = false;
		removeHard(root->rtChild, minNode->data);
	}
	else
	{
		FHlazySearchTreeNode<Comparable> *nodeToRemoveHard = root;
		root = (root->lftChild != NULL) ? root->lftChild : root->rtChild;
		delete nodeToRemoveHard;
	}
	return true;
}

template <class Comparable>
FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findHardMin(
	FHlazySearchTreeNode<Comparable> *root) const
{
	if (root == NULL)
		return NULL;
	if (root->lftChild == NULL)
		return root;
	return findHardMin(root->lftChild);
}
#endif

/*--------//--------//--------//--------//--------//--------//--------//--------
SAMPLE RUNS
--------//--------//--------//--------//--------//--------//--------//--------
PROVIDED MAIN:

initial size: 0
After populating -- traversal and sizes:
10 20 30 50 60 70
tree 1 size: 6  Hard size: 6
Collecting garbage on new tree - should be no garbage.
tree 1 size: 6  Hard size: 6


Attempting 1 removal:
removed 20
tree 1 size: 5  Hard size: 6
Collecting Garbage - should clean 1 item.
tree 1 size: 5  Hard size: 5
Collecting Garbage again - no change expected.
tree 1 size: 5  Hard size: 5
Adding 'hard' 22 - should see new sizes.
10 22 30 50 60 70
tree 1 size: 6  Hard size: 6

After soft removal.
10 30 50 60 70
tree 1 size: 5  Hard size: 6
Repeating soft removal. Should see no change.
10 30 50 60 70
tree 1 size: 5  Hard size: 6
Soft insertion. Hard size should not change.
10 22 30 50 60 70
tree 1 size: 6  Hard size: 6


Attempting 100 removals:
removed 70
removed 60
removed 50
removed 30
removed 22
removed 10

searchTree now:

tree 1 size: 0  Hard size: 0

searchTree2:
10 20 30 50 60 70 100 200 300 500 600 700
tree 2 size: 12  Hard size: 12

--------//--------//--------//--------//--------//--------//--------//--------
MY MAIN:

INITIALIZED TREE 1, A FHLAZYSEARCHTREE OBJECT.
initial size: 0
CALLING FINDMIN() AND FINDMAX() IN A HARD EMPTY TREE.
EmptyTreeException triggered.


INSERTING: 0
INSERTING: 10
INSERTING: -20
INSERTING: 30
INSERTING: -40
INSERTING: 50
INSERTING: -60
INSERTING: 70
INSERTING: -80
INSERTING: 90


TESTING FIND IN TREE 1
TRYING TO FIND 500
Not Found Exception triggered


INSERTED 10 ITEMS.
TREE 1 -- traversal and sizes:
-80 -60 -40 -20 0 10 30 50 70 90
tree 1 size: 10  Hard size: 10
Collecting garbage on tree 1 - should be no garbage.
tree 1 size: 10  Hard size: 10


TRYING FINDMIN() AND FINDMAX() IN A FULL TREE, TREE 1.
CALLING FINDMIN() AND FINDMAX().
findMin() = -80
findMax() = 90


TESTING THE ASSIGNMENT OPERATOR
INITIALIZED TREE 2, A FHLAZYTREE OBJECT
ASSIGNED TREE 1 TO TREE 2
TREE 2 -- traversal and sizes:
-80 -60 -40 -20 0 10 30 50 70 90
tree 2 size: 10  Hard size: 10
Collecting garbage on tree 2 - should be no garbage.
tree 2 size: 10  Hard size: 10


DELETING A NON-EXISTING ITEM IN TREE 1.
TRYING TO REMOVE 91
TREE 1 -- traversal and sizes:
-80 -60 -40 -20 0 10 30 50 70 90
tree 1 size: 10  Hard size: 10
Collecting garbage on tree 1 - should be no garbage.
tree 1 size: 10  Hard size: 10


SOFT DELETING THE ENTIRE TREE 1.
SOFT DELETING: 0
SOFT DELETING: 10
SOFT DELETING: -20
SOFT DELETING: 30
SOFT DELETING: -40
SOFT DELETING: 50
SOFT DELETING: -60
SOFT DELETING: 70
SOFT DELETING: -80
SOFT DELETING: 90
TREE 1 -- traversal and sizes:

tree 1 size: 0  Hard size: 10


TRYING FINDMIN() AND FINDMAX() IN A SOFT DELETED TREE, TREE 1.
CALLING FINDMIN() AND FINDMAX().
findMin() = 0
findMax() = 0


COLLECTGARBAGE() ON THE FULLY SOFT DELETED TREE 1
.Collecting garbage on tree 1 - should be no garbage.
tree 1 size: 0  Hard size: 0


ATTEMPTING A REMOVAL ON EMPTY TREE 1:
tree 1 size: 0  Hard size: 0
Collecting Garbage - no change expected.
tree 1 size: 0  Hard size: 0


Adding 'hard' 22 - should see new sizes.
22
tree 1 size: 1  Hard size: 1


SOFT REMOVING 22.
tree 1 size: 0  Hard size: 1


Repeating soft removal. Should see no change.
tree 1 size: 0  Hard size: 1


Soft insertion. Hard size should not change.
22
tree 1 size: 1  Hard size: 1


Attempting 100 removals:
removed 22


searchTree now:
tree 1 size: 0  Hard size: 0


searchTree2 now:
-80 -60 -40 -20 0 10 30 50 70 90
tree 2 size: 10  Hard size: 10


INSERTING THE SAME ITEMS INTO TREE 2.
INSERTING: 0
INSERTING: 10
INSERTING: -20
INSERTING: 30
INSERTING: -40
INSERTING: 50
INSERTING: -60
INSERTING: 70
INSERTING: -80
INSERTING: 90


searchTree2 now:
-80 -60 -40 -20 0 10 30 50 70 90
tree 2 size: 10  Hard size: 10


INSERTING THE DIFFERENT ITEMS INTO TREE 2.
INSERTING: -5
INSERTING: 15
INSERTING: -25
INSERTING: 35
INSERTING: -45
INSERTING: 55
INSERTING: -65
INSERTING: 75
INSERTING: -85
INSERTING: 95


searchTree2 now:
-85 -80 -65 -60 -45 -40 -25 -20 -5 0 10 15 30 35 50 55 70 75 90 95
tree 2 size: 20  Hard size: 20


TRYING FINDMIN() AND FINDMAX() IN A FULL TREE, TREE 2.
CALLING FINDMIN() AND FINDMAX().
findMin() = -85
findMax() = 95


SOFT DELETING SOME ITEMS IN TREE 2:
SOFT DELETING: 0
SOFT DELETING: 10
SOFT DELETING: -20
SOFT DELETING: 30
SOFT DELETING: -40
SOFT DELETING: 50
SOFT DELETING: -60
SOFT DELETING: 70
SOFT DELETING: -80
SOFT DELETING: 90


searchTree2 now:
-85 -65 -45 -25 -5 15 35 55 75 95
tree 2 size: 10  Hard size: 20


TESTING FIND IN TREE 2
TRYING TO FIND -5
NO EXCEPTION TRIGGERED, -5 WAS FOUND

TRYING FINDMIN() AND FINDMAX() IN A PARTIAL TREE, TREE 2.
CALLING FINDMIN() AND FINDMAX().
findMin() = -85
findMax() = 95


ASSIGNING NEW MIN AND MAX TO TREE 2
INSERTING: -500
INSERTING: 500


searchTree2 now:
-500 -85 -65 -45 -25 -5 15 35 55 75 95 500
tree 2 size: 12  Hard size: 22


TRYING FINDMIN() AND FINDMAX() IN A PARTIAL TREE, TREE 2.
CALLING FINDMIN() AND FINDMAX().
findMin() = -500
findMax() = 500


TESTING FIND IN TREE 2
TRYING TO FIND 500
NO EXCEPTION TRIGGERED, 500 WAS FOUND

TESTING FIND IN TREE 2
TRYING TO FIND 555
Not Found Exception triggered


COLLECTING GARBAGE IN TREE 2.


searchTree2 now:
-500 -85 -65 -45 -25 -5 15 35 55 75 95 500
tree 2 size: 12  Hard size: 12


TRYING FINDMIN() AND FINDMAX() IN A PARTIAL TREE, TREE 2.
CALLING FINDMIN() AND FINDMAX().
findMin() = -500
findMax() = 500

---------------------------------------------------------------------- */