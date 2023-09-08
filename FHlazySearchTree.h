//#ifndef FHLAZYSEARCHTREE_H
//#define FHLAZYSEARCHTREE_H
//#include <iostream>
//using namespace std;
//
//// ---------------------- FHlazySearchTreeNode Prototype -----------------------
//template <class Comparable>
//class FHlazySearchTreeNode
//{
//public:
//	// Addition of deleted to the init list.
//	FHlazySearchTreeNode(const Comparable & d = Comparable(),
//		FHlazySearchTreeNode *lt = NULL,
//		FHlazySearchTreeNode *rt = NULL,
//		bool dltd = false)
//		: lftChild(lt), rtChild(rt), data(d), deleted(dltd)
//	{ }
//	FHlazySearchTreeNode *lftChild, *rtChild;
//	Comparable data;
//	// New deleted member.
//	bool deleted;
//	// for use only with AVL Trees
//	virtual int getHeight() const { return 0; }
//	virtual bool setHeight(int height) { return true; }
//};
//
//// ---------------------- FHlazySearchTree Prototype --------------------------
//template <class Comparable>
//class FHlazySearchTree
//{
//protected:
//	// New mSizeHard member.
//	int mSize, mSizeHard;
//	FHlazySearchTreeNode<Comparable> *mRoot;
//
//public:
//	FHlazySearchTree() { mSize = 0; mRoot = NULL; }
//	FHlazySearchTree(const FHlazySearchTree &rhs)
//	{
//		mRoot = NULL; mSize = 0; *this = rhs;
//	}
//	~FHlazySearchTree() { clear(); }
//	const Comparable &findMin() const;
//	const Comparable &findMax() const;
//	const Comparable &find(const Comparable &x) const;
//	bool empty() const { return (mSize == 0); }
//	int size() const { return mSize; }
//	void clear() { makeEmpty(mRoot); }
//	const FHlazySearchTree & operator=(const FHlazySearchTree &rhs);
//	bool insert(const Comparable &x);
//	bool remove(const Comparable &x);
//	bool contains(const Comparable &x) const { return find(mRoot, x) != NULL; }
//	template <class Processor>
//	void traverse(Processor func) const { traverse(mRoot, func); }
//	int showHeight() const { return findHeight(mRoot); }
//	// Public, wrapper collectGarbage().
//	void collectGarbage() { collectGarbage(mRoot); }
//	// Accessor for mSizeHard.
//	int sizeHard() const { return mSizeHard; }
//
//protected:
//	FHlazySearchTreeNode<Comparable> *clone(
//		FHlazySearchTreeNode<Comparable> *root) const;
//	FHlazySearchTreeNode<Comparable> *findMin(
//		FHlazySearchTreeNode<Comparable> *root) const;
//	FHlazySearchTreeNode<Comparable> *findMax(
//		FHlazySearchTreeNode<Comparable> *root) const;
//	FHlazySearchTreeNode<Comparable> *find(
//		FHlazySearchTreeNode<Comparable> *root,
//		const Comparable &x) const;
//	bool insert(FHlazySearchTreeNode<Comparable> * &root,
//		const Comparable &x);
//	bool remove(FHlazySearchTreeNode<Comparable> * &root, const Comparable &x);
//	void makeEmpty(FHlazySearchTreeNode<Comparable> * &subtreeToDelete);
//	template <class Processor>
//	void traverse(FHlazySearchTreeNode<Comparable> *treeNode,
//		Processor func, int level = -1) const;
//	int findHeight(
//		FHlazySearchTreeNode<Comparable> *treeNode, int height = -1) const;
//	// Modified remove(), removeHard().
//	bool removeHard(FHlazySearchTreeNode<Comparable> * &root,
//		const Comparable &x);
//	FHlazySearchTreeNode<Comparable> *findHardMin(
//		FHlazySearchTreeNode<Comparable> *root) const;
//
//private:
//	// Private, recursive collectGarbage().
//	void collectGarbage(FHlazySearchTreeNode<Comparable> * &root);
//
//public:
//	// for exception throwing
//	class EmptyTreeException {};
//	class NotFoundException {};
//};
//
//// FHlazySearchTree public method definitions -----------------------------
//template <class Comparable>
//const Comparable & FHlazySearchTree<Comparable>::findMin() const
//{
//	if (mRoot == NULL)
//		throw EmptyTreeException();
//	return findMin(mRoot)->data;
//}
//
//template <class Comparable>
//const Comparable & FHlazySearchTree<Comparable>::findMax() const
//{
//	if (mRoot == NULL)
//		throw EmptyTreeException();
//	return findMax(mRoot)->data;
//}
//
//template <class Comparable>
//const Comparable &FHlazySearchTree<Comparable>::find(
//	const Comparable &x) const
//{
//	FHlazySearchTreeNode<Comparable> *resultNode;
//	resultNode = find(mRoot, x);
//	if (resultNode == NULL)
//		throw NotFoundException();
//	return resultNode->data;
//}
//
//template <class Comparable>
//const FHlazySearchTree<Comparable> &FHlazySearchTree<Comparable>::operator=
//(const FHlazySearchTree &rhs)
//{
//	if (&rhs != this)
//	{
//		clear();
//		mRoot = clone(rhs.mRoot);
//		mSize = rhs.size();
//		mSizeHard = rhs.sizeHard();
//	}
//	return *this;
//}
//
//template <class Comparable>
//bool FHlazySearchTree<Comparable>::insert(const Comparable &x)
//{
//	if (insert(mRoot, x))
//	{
//		mSize++;
//		return true;
//	}
//	return false;
//}
//
//template <class Comparable>
//bool FHlazySearchTree<Comparable>::remove(const Comparable &x)
//{
//	if (remove(mRoot, x))
//	{
//		mSize--;
//		return true;
//	}
//	return false;
//}
//
//template <class Comparable>
//template <class Processor>
//void FHlazySearchTree<Comparable>::traverse(
//	FHlazySearchTreeNode<Comparable> *treeNode,
//	Processor func, int level) const
//{
//	if (treeNode == NULL)
//		return;
//	// we're not doing anything with level but its there in case we want it
//	traverse(treeNode->lftChild, func, level + 1);
//	// We check for deletion.
//	if (!treeNode->deleted)
//		func(treeNode->data);
//	traverse(treeNode->rtChild, func, level + 1);
//}
//
//// FHlazySearchTree private method definitions -----------------------------
//template <class Comparable>
//FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::clone(
//	FHlazySearchTreeNode<Comparable> *root) const
//{
//	FHlazySearchTreeNode<Comparable> *newNode;
//	if (root == NULL)
//		return NULL;
//	// We also clone the deleted attribute.
//	newNode = new FHlazySearchTreeNode<Comparable>(
//		root->data,
//		clone(root->lftChild),
//		clone(root->rtChild),
//		root->deleted);
//	return newNode;
//}
//
//template <class Comparable>
//FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findMin(
//	FHlazySearchTreeNode<Comparable> *root) const
//{
//	if (root == NULL)
//		return NULL;
//	// Local variables belong to their scope, so we can create temp variables!
//	FHlazySearchTreeNode<Comparable> *lastNotDeleted = NULL;
//	// lastNotDeleted points to the last not deleted node traversed.
//	if (!root->deleted)
//		lastNotDeleted = root;
//	if (root->lftChild == NULL)
//	{
//		if (lastNotDeleted == NULL)
//		{// Pointing at a new default node so we dont get any runtime errors.
//			lastNotDeleted = new FHlazySearchTreeNode<Comparable>;
//		}
//		return lastNotDeleted;
//	}
//	return findMin(root->lftChild);
//}
//
//template <class Comparable>
//FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findMax(
//	FHlazySearchTreeNode<Comparable> *root) const
//{
//	if (root == NULL)
//		return NULL;
//	// Local variables belong to their scope, so we can create temp variables!
//	FHlazySearchTreeNode<Comparable> *lastNotDeleted = NULL;
//	// lastNotDeleted points to the last not deleted node traversed.
//	if (!root->deleted)
//		lastNotDeleted = root;
//	if (root->rtChild == NULL)
//	{
//		if (lastNotDeleted == NULL)
//			// Pointing at a new default node so we dont get any runtime errors.
//			lastNotDeleted = new FHlazySearchTreeNode<Comparable>;
//		return lastNotDeleted;
//	}
//	return findMax(root->rtChild);
//}
//
//template <class Comparable>
//FHlazySearchTreeNode<Comparable>* FHlazySearchTree<Comparable>::find(
//	FHlazySearchTreeNode<Comparable> *root, const Comparable &x) const
//{
//	// We also check if the root is deleted.
//	if (root == NULL)
//		return NULL;
//	if (x < root->data)
//		return find(root->lftChild, x);
//	if (root->data < x)
//		return find(root->rtChild, x);
//	(!root->deleted) ? root : NULL;
//}
//
//template <class Comparable>
//bool FHlazySearchTree<Comparable>::insert(
//	FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
//{
//	if (root == NULL)
//	{
//		root = new FHlazySearchTreeNode<Comparable>(x, NULL, NULL);
//		++mSizeHard;
//		return true;
//	}
//	else if (x < root->data)
//		return insert(root->lftChild, x);
//	else if (root->data < x)
//		return insert(root->rtChild, x);
//	// If the root is soft deleted we just un-delete it.
//	else if (root->deleted)
//	{
//		root->deleted = false;
//		return true;
//	}
//	return false; // duplicate
//}
//
//template <class Comparable>
//bool FHlazySearchTree<Comparable>::remove(
//	FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
//{
//	if (root == NULL)
//		return false;
//	if (x < root->data)
//		return remove(root->lftChild, x);
//	if (root->data < x)
//		return remove(root->rtChild, x);
//	// If not deleted, mark it for deletion. DON'T hard delete it.
//	if (!root->deleted)
//	{
//		root->deleted = true;
//		return true;
//	}
//	return false;
//}
//
//template <class Comparable>
//void FHlazySearchTree<Comparable>::makeEmpty(
//	FHlazySearchTreeNode<Comparable> * &subtreeToDelete)
//{
//	if (subtreeToDelete == NULL)
//		return;
//	// remove children
//	makeEmpty(subtreeToDelete->lftChild);
//	makeEmpty(subtreeToDelete->rtChild);
//	// clear client's pointer
//	delete subtreeToDelete;
//	subtreeToDelete = NULL;
//	// New size conditions.
//	if (!subtreeToDelete->deleted)
//		--mSize;
//	--mSizeHard;
//}
//
//template <class Comparable>
//int FHlazySearchTree<Comparable>::findHeight(
//	FHlazySearchTreeNode<Comparable> *treeNode, int height) const
//{
//	int leftHeight, rightHeight;
//	if (treeNode == NULL)
//		return height;
//	height++;
//	leftHeight = findHeight(treeNode->lftChild, height);
//	rightHeight = findHeight(treeNode->rtChild, height);
//	return (leftHeight > rightHeight) ? leftHeight : rightHeight;
//}
//
//template <class Comparable>
//void FHlazySearchTree<Comparable>::collectGarbage(
//	FHlazySearchTreeNode<Comparable> * &root)
//{
//	// We use the same algorithm as in makeEmpty(), as it traverses
//	// the entire tree and has similar purposes.
//	if (root == NULL)
//		return;
//	// remove children
//	collectGarbage(root->lftChild);
//	collectGarbage(root->rtChild);
//	// We simply cast the already defined removeHard() method.
//	if (root->deleted)
//	{
//		removeHard(root, root->data);
//		--mSizeHard;
//	}
//}
//
//// removeHard() is just a modified version of the original remove().
//template <class Comparable>
//bool FHlazySearchTree<Comparable>::removeHard(
//	FHlazySearchTreeNode<Comparable> * &root, const Comparable &x)
//{
//	if (root == NULL)
//		return false;
//	if (x < root->data)
//		return removeHard(root->lftChild, x);
//	if (root->data < x)
//		return removeHard(root->rtChild, x);
//	// found the node
//	if (root->lftChild != NULL && root->rtChild != NULL)
//	{
//		// Use the original findMin(), but with a different name.
//		FHlazySearchTreeNode<Comparable> *minNode = findHardMin(root->rtChild);
//		root->data = minNode->data;
//		// Update member info.
//		root->deleted = false;
//		removeHard(root->rtChild, minNode->data);
//	}
//	else
//	{
//		FHlazySearchTreeNode<Comparable> *nodeToRemoveHard = root;
//		root = (root->lftChild != NULL) ? root->lftChild : root->rtChild;
//		delete nodeToRemoveHard;
//	}
//	return true;
//}
//
//template <class Comparable>
//FHlazySearchTreeNode<Comparable> *FHlazySearchTree<Comparable>::findHardMin(
//	FHlazySearchTreeNode<Comparable> *root) const
//{
//	if (root == NULL)
//		return NULL;
//	if (root->lftChild == NULL)
//		return root;
//	return findHardMin(root->lftChild);
//}
//#endif