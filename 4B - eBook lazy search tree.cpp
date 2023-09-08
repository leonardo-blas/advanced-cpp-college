// Assignment 4, by Leonardo Blas.

#ifndef FHLAZYSEARCHTREE_H
#define FHLAZYSEARCHTREE_H

#include <iostream>
#include <string>
#include <stack>
#include "EBookEntry.h"

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

class PrintBook
{
public:
	void operator()(EBookEntry book)
	{
		cout << book.getETextNum() << ": " << book.getCreator().substr(0, 15)
			<< ", " << book.getTitle().substr(0, 20)
			<< ", " << book.getSubject().substr(0, 25) << endl;
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
	int k, arraySize;
	FHlazySearchTree<EBookEntry> lazyTree;
	PrintBook bookPrinter;

	// read the data from file
	EBookEntryReader bookInput("catalog-short4.txt");
	if (bookInput.readError())
	{
		cout << "couldn't open " << bookInput.getFileName() << " for input.\n";
		exit(1);
	}

	// changing the sort key to SORT_BY_TITLE will not allow duplicate titles, e.g.
	EBookEntry::setSortType(EBookEntry::SORT_BY_ID);
	arraySize = bookInput.getNumBooks();

	// build the tree
	for (k = 0; k < arraySize; k++)
		if (!lazyTree.insert(bookInput[k]))
		{
			cout << "NOT INSERTED: ";
			bookPrinter(bookInput[k]);
		}

	// confirm the size:
	cout << "Num Books: " << arraySize
		<< ", Tree Size: " << lazyTree.size() 
		<< "  Hard size: " << lazyTree.sizeHard() << endl;

	// test finds
	int START = 1000, STOP = 1020;
	cout << "\n\nAttempting " << STOP - START << " finds: \n";
	for (k = START; k < STOP; k++)
		if (!lazyTree.contains(bookInput[k]))
			cout << " !!! NOT FOUND: " << k << ": " << bookInput[k].getTitle() << endl;
		else
		{
			cout << "Found: ";
			bookPrinter(bookInput[k]);
		}

	int START3 = 0, STOP3 = 4800;
	cout << "\n\nAttempting " << STOP3 - START3 << " removals: \n";
	for (k = START3; k < STOP3; k++)
		if (!lazyTree.remove(bookInput[k]))
			cout << " !!!NOT FOUND: " << k << ": " << bookInput[k].getTitle() << endl;

	cout << "\n\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard() << endl;

	cout << "\n\nCOLLECTING GARBAGE IN TREE.\n";
	lazyTree.collectGarbage();
	cout << "\n\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard() << endl;

	int START4 = 4790, STOP4 = 4800;
	cout << "\n\nAttempting " << STOP4 - START4 << " removals: \n";
	for (k = START4; k < STOP4; k++)
		if (!lazyTree.remove(bookInput[k]))
			cout << " !!!NOT FOUND: " << k << ": " << bookInput[k].getTitle() << endl;

	cout << "\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard() << endl;

	cout << "\n\nAttempting " << STOP4 - START4 << " INSERTS: \n";
	for (k = START4; k < STOP4; k++)
		if (!lazyTree.insert(bookInput[k]))
			cout << " !!!NOT INSERTED: " << k << ": " << bookInput[k].getTitle() << endl;
		else
		{
			cout << "INSERTED: ";
			bookPrinter(bookInput[k]);
		}

	cout << "\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard()
		<< "\nTree structure:\n";
	lazyTree.traverse(bookPrinter);
	
	int START5 = 4840, STOP5 = 4895;
	cout << "\n\nAttempting " << STOP5 - START5 << " removals: \n";
	for (k = START5; k < STOP5; k++)
		if (!lazyTree.remove(bookInput[k]))
			cout << " !!!NOT FOUND: " << k << ": " << bookInput[k].getTitle() << endl;

	cout << "\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard()
		<< "\nTree structure:\n";
	lazyTree.traverse(bookPrinter);

	cout << "\n\nCOLLECTING GARBAGE IN TREE.\n";
	lazyTree.collectGarbage();

	cout << "\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard()
		<< "\nTree structure:\n";
	lazyTree.traverse(bookPrinter);

	int START7 = 0, STOP7 = 15;
	cout << "\n\nAttempting " << STOP7 - START7 << " INSERTS: \n";
	for (k = START7; k < STOP7; k++)
		if (!lazyTree.insert(bookInput[k]))
			cout << " !!!NOT INSERTED: " << k << ": " << bookInput[k].getTitle() << endl;
		else
		{
			cout << "INSERTED: ";
			bookPrinter(bookInput[k]);
		}

	cout << "\n\nCOLLECTING GARBAGE IN TREE.\n";
	lazyTree.collectGarbage();

	cout << "\nTree size: " << lazyTree.size()
		<< "  Hard size: " << lazyTree.sizeHard()
		<< "\nTree structure:\n";
	lazyTree.traverse(bookPrinter);

	/*
	// this will print thousands of books, so be careful
	cout << "\nsearch_tree after deletes:\n";
	lazyTree.traverse(bookPrinter);
	*/

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

Num Books: 4863, Tree Size: 4863


Attempting 20 finds:
Found: 23574: La Monte, Rober, Socialism: Positive , (no data found)
Found: 22740: Unknown, The Apple Dumpling a, (no data found)
Found: 22693: Lang, Jeanie, A Book of Myths, Mythology
Found: 28486: Cole, Everett B, The Weakling, Science fiction
Found: 28488: Moli├¿re, 1622-, Tartuffeor The Hypoc, Comedies
Found: 28489: Locke, William , The Belov├⌐d Vagabon, PR
Found: 28490: Girl Scouts of , Scouting For Girls, , Girl Scouts -- Handbooks,
Found: 23448: Brown, Ruth Alb, Heart of Gold, People with disabilities
Found: 22599: Karpinski, Loui, The Hindu-Arabic Num, Numerals
Found: 23337: Ing, Dean Charl, Tight Squeeze, Science fiction
Found: 22360: Bradley, Mary H, The Fortieth Door, PS
Found: 23293: Lawrence, Rober, Primitive Psycho-The, (no data found)
Found: 28066: Various, The Christian Founda, Religion and science -- P
Found: 28067: Smith, J. Allen, The Spirit of Americ, Constitutional history --
Found: 28069: Bangs, John Ken, Alice in Blunderland, Fantasy
Found: 28070: Harriman, Alice, A Man of Two Countri, PS
Found: 23205: Various, Notes and Queries, N, Questions and answers --
Found: 22171: Ernst, Paul, 18, The Radiant Shell, Science fiction
Found: 23075: Baum, L. Frank , Ozma of Oz, Oz (Imaginary place) -- F
Found: 29230: Barker, James N, The Indian PrincessL, American drama


Attempting 20 removals:

new size: 4843

--------//--------//--------//--------//--------//--------//--------//--------
MY MAIN:

Num Books: 4863, Tree Size: 4863  Hard size: 4863


Attempting 20 finds:
Found: 23574: La Monte, Rober, Socialism: Positive , (no data found)
Found: 22740: Unknown, The Apple Dumpling a, (no data found)
Found: 22693: Lang, Jeanie, A Book of Myths, Mythology
Found: 28486: Cole, Everett B, The Weakling, Science fiction
Found: 28488: Moli├¿re, 1622-, Tartuffeor The Hypoc, Comedies
Found: 28489: Locke, William , The Belov├⌐d Vagabon, PR
Found: 28490: Girl Scouts of , Scouting For Girls, , Girl Scouts -- Handbooks,
Found: 23448: Brown, Ruth Alb, Heart of Gold, People with disabilities
Found: 22599: Karpinski, Loui, The Hindu-Arabic Num, Numerals
Found: 23337: Ing, Dean Charl, Tight Squeeze, Science fiction
Found: 22360: Bradley, Mary H, The Fortieth Door, PS
Found: 23293: Lawrence, Rober, Primitive Psycho-The, (no data found)
Found: 28066: Various, The Christian Founda, Religion and science -- P
Found: 28067: Smith, J. Allen, The Spirit of Americ, Constitutional history --
Found: 28069: Bangs, John Ken, Alice in Blunderland, Fantasy
Found: 28070: Harriman, Alice, A Man of Two Countri, PS
Found: 23205: Various, Notes and Queries, N, Questions and answers --
Found: 22171: Ernst, Paul, 18, The Radiant Shell, Science fiction
Found: 23075: Baum, L. Frank , Ozma of Oz, Oz (Imaginary place) -- F
Found: 29230: Barker, James N, The Indian PrincessL, American drama


Attempting 4800 removals:


Tree size: 63  Hard size: 4863


COLLECTING GARBAGE IN TREE.


Tree size: 63  Hard size: 63


Attempting 10 removals:
!!!NOT FOUND: 4790: Samuel Richardson's Introduction to Pamela
!!!NOT FOUND: 4791: Turned Adrift
!!!NOT FOUND: 4792: The Story of Wool
!!!NOT FOUND: 4793: Journeys Through Bookland, Vol. 10The Guide
!!!NOT FOUND: 4794: Odysseus, the Hero of IthacaAdapted from the Third Book of the Primary Schools of Athens, Greece
!!!NOT FOUND: 4795: A Little Present for a Good Child
!!!NOT FOUND: 4796: The Defence of Duffer's Drift
!!!NOT FOUND: 4797: Friendly Visiting among the PoorA Handbook for Charity Workers
!!!NOT FOUND: 4798: The Blonde LadyBeing a Record of the Duel of Wits between Ars├¿ne Lupin and the English Detective
!!!NOT FOUND: 4799: Tessa1901

Tree size: 63  Hard size: 63


Attempting 10 INSERTS:
INSERTED: 24860: Richardson, Sam, Samuel Richardson's , Richardson, Samuel, 1689-
INSERTED: 24859: Collingwood, Ha, Turned Adrift, Survival after airplane a
INSERTED: 24858: Bassett, Sara W, The Story of Wool, Sheep -- Juvenile literat
INSERTED: 24857: Sylvester, Char, Journeys Through Boo, Children's literature
INSERTED: 24856: Homer, 750? BC-, Odysseus, the Hero o, Classical literature
INSERTED: 24849: Unknown, A Little Present for, Children's poetry
INSERTED: 24842: Swinton, Ernest, The Defence of Duffe, Military art and science
INSERTED: 24841: Richmond, Mary , Friendly Visiting am, Friendly visiting
INSERTED: 24839: Leblanc, Mauric, The Blonde LadyBeing, Detective and mystery sto
INSERTED: 24838: Becke, Louis, 1, Tessa1901, Pacific Area -- Social li

Tree size: 73  Hard size: 73
Tree structure:
24742: Stephens, James, Mary, Mary, Mothers and daughters --
24745: McGregor, Dunca, The Loss of the Kent, Shipwrecks
24749: Reynolds, Mack,, Adaptation, Science fiction, American
24750: Lane, Mary E. B, Mizora: A ProphecyA , Science fiction
24751: Walton, Amy, 18, The Kitchen Cat and , Short stories
24752: Achenwall, Gott, Anmerkungen ├╝ber No, United States -- Descript
24753: Arthur, T. S. (, Who Are Happiest? an, Christian life -- Juvenil
24754: Cervantes Saave, Wit and Wisdom of Do, Cervantes Saavedra, Migue
24755: Kingston, Willi, Captain CookHis Life, Biography
24756: Lang, Jeanie, The Story of General, Gordon, Charles George, 1
24758: Altsheler, Jose, The Eyes of the Wood, Frontier and pioneer life
24759: Burbidge, Edwar, The Kingdom of Heave, Heaven -- Biblical teachi
24760: Various, Aunt Kitty's Stories, Children's poetry
24761: Sheridan, Richa, The RivalsA Comedy, English drama (Comedy)
24762: Tappan, Eva Mar, Diggers in the Earth, Readers
24764: Griffith, Georg, The World Peril of 1, Science fiction, English
24765: Lippitt, Franci, A Treatise on the Ta, United States -- History
24767: Wallace, Edgar,, Jack O' Judgment, Detective and mystery sto
24770: Sinclair, Upton, A Prisoner of MorroI, Spanish-American War, 189
24772: Burnett, France, Sara Creweor, What H, Orphans -- Fiction
24775: Optic, Oliver, , Up the Riveror, Yach, Yachts -- Juvenile fictio
24776: Various, The Brochure Series , Architecture -- Periodica
24777: Verne, Jules, 1, Celebrated Travels a, Voyages and travels
24778: Unknown, The National Nursery, Fairy tales
24779: Cole, Everett B, Millennium, Science fiction, American
24780: Victoria, Queen, The Letters of Queen, Great Britain -- History
24781: Baerlein, Henry, The Birth of Yugosla, Yugoslavia
24783: Calthrop, Dion , The Pirate's Pocket , Wit and humor
24784: Turnbull, Lawre, The Royal Pawn of Ve, Historical fiction
24785: Phillips, Steph, Nero, Nero, Emperor of Rome, 37
24786: (no data found), Games and Play for S, Games
24787: Black, C. B., -, The South of FranceΓ, France -- Description and
24788: Mooney, James, , The Sacred Formulas , Cherokee language -- Text
24789: Orczy, Emmuska , Orczy, Emmuska Orczy, Historical fiction
24790: (no data found), Early English Meals , Etiquette
24793: Day, Holman, 18, Blow The Man DownA R, Ship captains -- Fiction
24794: Bell, Florence , The ArbiterA Novel, Great Britain -- History
24795: Unknown, An Entertaining Hist, Fairy tales
24798: (no data found), America FirstPatriot, United States
24799: Cobb, Irvin S. , The Escape of Mr. Tr, Short stories
24800: Romanes, George, Darwin, and After Da, Evolution (Biology)
24803: Various, Notes and Queries, 1, Questions and answers --
24804: Gould, Nat, 185, The Rider in KhakiA , England -- Fiction
24805: Becke, Louis, 1, From: Chinkie's Flat, Pacific Area -- Social li
24806: Becke, Louis, 1, John Frewen, South S, Whaling -- Fiction
24808: Wood, William C, All AfloatA Chronicl, Canada -- History
24811: Hall, Jennie, 1, Viking Tales, Short stories
24813: Fenn, George Ma, The Queen's ScarletT, Cousins -- Fiction
24814: Reid, Mayne, 18, The Forest ExilesThe, Adventure stories
24815: Davis, Edward Z, Translations of Germ, German poetry -- Translat
24816: Leighton, Carol, Life at Puget Sound:, California -- Description
24818: Freeman, Edward, Sketches of Travel i, Normandy (France) -- Desc
24821: Fenn, George Ma, Diamond DykeThe Lone, South Africa -- Fiction
24822: Jewett, Sarah O, The Queen's Twin and, Short stories
24825: Anonymous, The Ghost of Chatham, Poetry
24826: Richards, Laura, Hildegarde's Holiday, Conduct of life -- Juveni
24827: Richards, Laura, Rita, Adventure and adventurers
24828: Richards, Laura, Margaret Montfort, Family -- Fiction
24829: Hamilton, Frede, The Uses of ItalicA , Printing -- Style manuals
24831: Abbott, Jacob, , Forests of MaineMarc, Forests and forestry -- M
24835: Becke, Louis, 1, R├¡dan The Devil And, Short stories
24836: Becke, Louis, 1, Rodman The Boatsteer, Short stories
24837: Becke, Louis, 1, The Trader's Wife190, Pacific Area -- Social li
24838: Becke, Louis, 1, Tessa1901, Pacific Area -- Social li
24839: Leblanc, Mauric, The Blonde LadyBeing, Detective and mystery sto
24841: Richmond, Mary , Friendly Visiting am, Friendly visiting
24842: Swinton, Ernest, The Defence of Duffe, Military art and science
24849: Unknown, A Little Present for, Children's poetry
24856: Homer, 750? BC-, Odysseus, the Hero o, Classical literature
24857: Sylvester, Char, Journeys Through Boo, Children's literature
24858: Bassett, Sara W, The Story of Wool, Sheep -- Juvenile literat
24859: Collingwood, Ha, Turned Adrift, Survival after airplane a
24860: Richardson, Sam, Samuel Richardson's , Richardson, Samuel, 1689-


Attempting 55 removals:
!!!NOT FOUND: 4863:
!!!NOT FOUND: 4864:
!!!NOT FOUND: 4865:
!!!NOT FOUND: 4866:
!!!NOT FOUND: 4867:
!!!NOT FOUND: 4868:
!!!NOT FOUND: 4869:
!!!NOT FOUND: 4870:
!!!NOT FOUND: 4871:
!!!NOT FOUND: 4872:
!!!NOT FOUND: 4873:
!!!NOT FOUND: 4874:
!!!NOT FOUND: 4875:
!!!NOT FOUND: 4876:
!!!NOT FOUND: 4877:
!!!NOT FOUND: 4878:
!!!NOT FOUND: 4879:
!!!NOT FOUND: 4880:
!!!NOT FOUND: 4881:
!!!NOT FOUND: 4882:
!!!NOT FOUND: 4883:
!!!NOT FOUND: 4884:
!!!NOT FOUND: 4885:
!!!NOT FOUND: 4886:
!!!NOT FOUND: 4887:
!!!NOT FOUND: 4888:
!!!NOT FOUND: 4889:
!!!NOT FOUND: 4890:
!!!NOT FOUND: 4891:
!!!NOT FOUND: 4892:
!!!NOT FOUND: 4893:
!!!NOT FOUND: 4894:

Tree size: 50  Hard size: 73
Tree structure:
24778: Unknown, The National Nursery, Fairy tales
24779: Cole, Everett B, Millennium, Science fiction, American
24780: Victoria, Queen, The Letters of Queen, Great Britain -- History
24781: Baerlein, Henry, The Birth of Yugosla, Yugoslavia
24783: Calthrop, Dion , The Pirate's Pocket , Wit and humor
24784: Turnbull, Lawre, The Royal Pawn of Ve, Historical fiction
24785: Phillips, Steph, Nero, Nero, Emperor of Rome, 37
24786: (no data found), Games and Play for S, Games
24787: Black, C. B., -, The South of FranceΓ, France -- Description and
24788: Mooney, James, , The Sacred Formulas , Cherokee language -- Text
24789: Orczy, Emmuska , Orczy, Emmuska Orczy, Historical fiction
24790: (no data found), Early English Meals , Etiquette
24793: Day, Holman, 18, Blow The Man DownA R, Ship captains -- Fiction
24794: Bell, Florence , The ArbiterA Novel, Great Britain -- History
24795: Unknown, An Entertaining Hist, Fairy tales
24798: (no data found), America FirstPatriot, United States
24799: Cobb, Irvin S. , The Escape of Mr. Tr, Short stories
24800: Romanes, George, Darwin, and After Da, Evolution (Biology)
24803: Various, Notes and Queries, 1, Questions and answers --
24804: Gould, Nat, 185, The Rider in KhakiA , England -- Fiction
24805: Becke, Louis, 1, From: Chinkie's Flat, Pacific Area -- Social li
24806: Becke, Louis, 1, John Frewen, South S, Whaling -- Fiction
24808: Wood, William C, All AfloatA Chronicl, Canada -- History
24811: Hall, Jennie, 1, Viking Tales, Short stories
24813: Fenn, George Ma, The Queen's ScarletT, Cousins -- Fiction
24814: Reid, Mayne, 18, The Forest ExilesThe, Adventure stories
24815: Davis, Edward Z, Translations of Germ, German poetry -- Translat
24816: Leighton, Carol, Life at Puget Sound:, California -- Description
24818: Freeman, Edward, Sketches of Travel i, Normandy (France) -- Desc
24821: Fenn, George Ma, Diamond DykeThe Lone, South Africa -- Fiction
24822: Jewett, Sarah O, The Queen's Twin and, Short stories
24825: Anonymous, The Ghost of Chatham, Poetry
24826: Richards, Laura, Hildegarde's Holiday, Conduct of life -- Juveni
24827: Richards, Laura, Rita, Adventure and adventurers
24828: Richards, Laura, Margaret Montfort, Family -- Fiction
24829: Hamilton, Frede, The Uses of ItalicA , Printing -- Style manuals
24831: Abbott, Jacob, , Forests of MaineMarc, Forests and forestry -- M
24835: Becke, Louis, 1, R├¡dan The Devil And, Short stories
24836: Becke, Louis, 1, Rodman The Boatsteer, Short stories
24837: Becke, Louis, 1, The Trader's Wife190, Pacific Area -- Social li
24838: Becke, Louis, 1, Tessa1901, Pacific Area -- Social li
24839: Leblanc, Mauric, The Blonde LadyBeing, Detective and mystery sto
24841: Richmond, Mary , Friendly Visiting am, Friendly visiting
24842: Swinton, Ernest, The Defence of Duffe, Military art and science
24849: Unknown, A Little Present for, Children's poetry
24856: Homer, 750? BC-, Odysseus, the Hero o, Classical literature
24857: Sylvester, Char, Journeys Through Boo, Children's literature
24858: Bassett, Sara W, The Story of Wool, Sheep -- Juvenile literat
24859: Collingwood, Ha, Turned Adrift, Survival after airplane a
24860: Richardson, Sam, Samuel Richardson's , Richardson, Samuel, 1689-


COLLECTING GARBAGE IN TREE.

Tree size: 50  Hard size: 50
Tree structure:
24778: Unknown, The National Nursery, Fairy tales
24779: Cole, Everett B, Millennium, Science fiction, American
24780: Victoria, Queen, The Letters of Queen, Great Britain -- History
24781: Baerlein, Henry, The Birth of Yugosla, Yugoslavia
24783: Calthrop, Dion , The Pirate's Pocket , Wit and humor
24784: Turnbull, Lawre, The Royal Pawn of Ve, Historical fiction
24785: Phillips, Steph, Nero, Nero, Emperor of Rome, 37
24786: (no data found), Games and Play for S, Games
24787: Black, C. B., -, The South of FranceΓ, France -- Description and
24788: Mooney, James, , The Sacred Formulas , Cherokee language -- Text
24789: Orczy, Emmuska , Orczy, Emmuska Orczy, Historical fiction
24790: (no data found), Early English Meals , Etiquette
24793: Day, Holman, 18, Blow The Man DownA R, Ship captains -- Fiction
24794: Bell, Florence , The ArbiterA Novel, Great Britain -- History
24795: Unknown, An Entertaining Hist, Fairy tales
24798: (no data found), America FirstPatriot, United States
24799: Cobb, Irvin S. , The Escape of Mr. Tr, Short stories
24800: Romanes, George, Darwin, and After Da, Evolution (Biology)
24803: Various, Notes and Queries, 1, Questions and answers --
24804: Gould, Nat, 185, The Rider in KhakiA , England -- Fiction
24805: Becke, Louis, 1, From: Chinkie's Flat, Pacific Area -- Social li
24806: Becke, Louis, 1, John Frewen, South S, Whaling -- Fiction
24808: Wood, William C, All AfloatA Chronicl, Canada -- History
24811: Hall, Jennie, 1, Viking Tales, Short stories
24813: Fenn, George Ma, The Queen's ScarletT, Cousins -- Fiction
24814: Reid, Mayne, 18, The Forest ExilesThe, Adventure stories
24815: Davis, Edward Z, Translations of Germ, German poetry -- Translat
24816: Leighton, Carol, Life at Puget Sound:, California -- Description
24818: Freeman, Edward, Sketches of Travel i, Normandy (France) -- Desc
24821: Fenn, George Ma, Diamond DykeThe Lone, South Africa -- Fiction
24822: Jewett, Sarah O, The Queen's Twin and, Short stories
24825: Anonymous, The Ghost of Chatham, Poetry
24826: Richards, Laura, Hildegarde's Holiday, Conduct of life -- Juveni
24827: Richards, Laura, Rita, Adventure and adventurers
24828: Richards, Laura, Margaret Montfort, Family -- Fiction
24829: Hamilton, Frede, The Uses of ItalicA , Printing -- Style manuals
24831: Abbott, Jacob, , Forests of MaineMarc, Forests and forestry -- M
24835: Becke, Louis, 1, R├¡dan The Devil And, Short stories
24836: Becke, Louis, 1, Rodman The Boatsteer, Short stories
24837: Becke, Louis, 1, The Trader's Wife190, Pacific Area -- Social li
24838: Becke, Louis, 1, Tessa1901, Pacific Area -- Social li
24839: Leblanc, Mauric, The Blonde LadyBeing, Detective and mystery sto
24841: Richmond, Mary , Friendly Visiting am, Friendly visiting
24842: Swinton, Ernest, The Defence of Duffe, Military art and science
24849: Unknown, A Little Present for, Children's poetry
24856: Homer, 750? BC-, Odysseus, the Hero o, Classical literature
24857: Sylvester, Char, Journeys Through Boo, Children's literature
24858: Bassett, Sara W, The Story of Wool, Sheep -- Juvenile literat
24859: Collingwood, Ha, Turned Adrift, Survival after airplane a
24860: Richardson, Sam, Samuel Richardson's , Richardson, Samuel, 1689-


Attempting 15 INSERTS:
INSERTED: 30170: Winterbotham, R, Lonesome Hearts, Science fiction
INSERTED: 30169: Unknown, The Story of the Whi, Conduct of life -- Juveni
INSERTED: 28546: Ranke, Leopold , A History of England, Great Britain -- History
INSERTED: 28711: Bacon, Mary Sch, Operas Every Child S, Operas -- Stories, plots,
INSERTED: 28805: Raymond, Evelyn, Dorothy's House Part, Orphans -- Juvenile ficti
INSERTED: 28547: Macduff, John R, The Words of Jesus, Devotional exercises
INSERTED: 30168: Bell, Edward Al, A History of Giggles, Giggleswick School (Giggl
INSERTED: 28712: Various, The American Mission, Congregational churches -
INSERTED: 28548: Smith, George, , Gipsy Lifebeing an a, Romanies -- Great Britain
INSERTED: 29171: Quiller-Couch, , The Carroll Girls, Girls -- Conduct of life
INSERTED: 28821: Warner, Charles, The Works of Charles, Indexes
INSERTED: 28549: Smith, Hugh, 17, A Treatise on Foreig, Tea -- Health aspects --
INSERTED: 30167: Nesbit, E. (Edi, Royal Children of En, Princes -- Great Britain
INSERTED: 30166: Various, Astounding Stories, , Science fiction -- Period
INSERTED: 28822: Churchill, Wins, The Works Of Winston, Indexes


COLLECTING GARBAGE IN TREE.

Tree size: 65  Hard size: 65
Tree structure:
24778: Unknown, The National Nursery, Fairy tales
24779: Cole, Everett B, Millennium, Science fiction, American
24780: Victoria, Queen, The Letters of Queen, Great Britain -- History
24781: Baerlein, Henry, The Birth of Yugosla, Yugoslavia
24783: Calthrop, Dion , The Pirate's Pocket , Wit and humor
24784: Turnbull, Lawre, The Royal Pawn of Ve, Historical fiction
24785: Phillips, Steph, Nero, Nero, Emperor of Rome, 37
24786: (no data found), Games and Play for S, Games
24787: Black, C. B., -, The South of FranceΓ, France -- Description and
24788: Mooney, James, , The Sacred Formulas , Cherokee language -- Text
24789: Orczy, Emmuska , Orczy, Emmuska Orczy, Historical fiction
24790: (no data found), Early English Meals , Etiquette
24793: Day, Holman, 18, Blow The Man DownA R, Ship captains -- Fiction
24794: Bell, Florence , The ArbiterA Novel, Great Britain -- History
24795: Unknown, An Entertaining Hist, Fairy tales
24798: (no data found), America FirstPatriot, United States
24799: Cobb, Irvin S. , The Escape of Mr. Tr, Short stories
24800: Romanes, George, Darwin, and After Da, Evolution (Biology)
24803: Various, Notes and Queries, 1, Questions and answers --
24804: Gould, Nat, 185, The Rider in KhakiA , England -- Fiction
24805: Becke, Louis, 1, From: Chinkie's Flat, Pacific Area -- Social li
24806: Becke, Louis, 1, John Frewen, South S, Whaling -- Fiction
24808: Wood, William C, All AfloatA Chronicl, Canada -- History
24811: Hall, Jennie, 1, Viking Tales, Short stories
24813: Fenn, George Ma, The Queen's ScarletT, Cousins -- Fiction
24814: Reid, Mayne, 18, The Forest ExilesThe, Adventure stories
24815: Davis, Edward Z, Translations of Germ, German poetry -- Translat
24816: Leighton, Carol, Life at Puget Sound:, California -- Description
24818: Freeman, Edward, Sketches of Travel i, Normandy (France) -- Desc
24821: Fenn, George Ma, Diamond DykeThe Lone, South Africa -- Fiction
24822: Jewett, Sarah O, The Queen's Twin and, Short stories
24825: Anonymous, The Ghost of Chatham, Poetry
24826: Richards, Laura, Hildegarde's Holiday, Conduct of life -- Juveni
24827: Richards, Laura, Rita, Adventure and adventurers
24828: Richards, Laura, Margaret Montfort, Family -- Fiction
24829: Hamilton, Frede, The Uses of ItalicA , Printing -- Style manuals
24831: Abbott, Jacob, , Forests of MaineMarc, Forests and forestry -- M
24835: Becke, Louis, 1, R├¡dan The Devil And, Short stories
24836: Becke, Louis, 1, Rodman The Boatsteer, Short stories
24837: Becke, Louis, 1, The Trader's Wife190, Pacific Area -- Social li
24838: Becke, Louis, 1, Tessa1901, Pacific Area -- Social li
24839: Leblanc, Mauric, The Blonde LadyBeing, Detective and mystery sto
24841: Richmond, Mary , Friendly Visiting am, Friendly visiting
24842: Swinton, Ernest, The Defence of Duffe, Military art and science
24849: Unknown, A Little Present for, Children's poetry
24856: Homer, 750? BC-, Odysseus, the Hero o, Classical literature
24857: Sylvester, Char, Journeys Through Boo, Children's literature
24858: Bassett, Sara W, The Story of Wool, Sheep -- Juvenile literat
24859: Collingwood, Ha, Turned Adrift, Survival after airplane a
24860: Richardson, Sam, Samuel Richardson's , Richardson, Samuel, 1689-
28546: Ranke, Leopold , A History of England, Great Britain -- History
28547: Macduff, John R, The Words of Jesus, Devotional exercises
28548: Smith, George, , Gipsy Lifebeing an a, Romanies -- Great Britain
28549: Smith, Hugh, 17, A Treatise on Foreig, Tea -- Health aspects --
28711: Bacon, Mary Sch, Operas Every Child S, Operas -- Stories, plots,
28712: Various, The American Mission, Congregational churches -
28805: Raymond, Evelyn, Dorothy's House Part, Orphans -- Juvenile ficti
28821: Warner, Charles, The Works of Charles, Indexes
28822: Churchill, Wins, The Works Of Winston, Indexes
29171: Quiller-Couch, , The Carroll Girls, Girls -- Conduct of life
30166: Various, Astounding Stories, , Science fiction -- Period
30167: Nesbit, E. (Edi, Royal Children of En, Princes -- Great Britain
30168: Bell, Edward Al, A History of Giggles, Giggleswick School (Giggl
30169: Unknown, The Story of the Whi, Conduct of life -- Juveni
30170: Winterbotham, R, Lonesome Hearts, Science fiction

/--------//--------//--------//--------//--------//--------//--------//------*/