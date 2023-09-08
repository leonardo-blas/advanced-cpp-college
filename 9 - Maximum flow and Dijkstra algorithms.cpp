// Assignment 9, by Leonardo Blas.

#ifndef FHGRAPH_H
#define FHGRAPH_H
#include <limits.h>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include <functional>
#include <string>
using namespace std;

// --------------- FHflowVertex Prototype ---------------
// CostType is some numeric type that expresses cost of edges
// Object is the non-graph data for a vertex
template <class Object, typename CostType>
class FHflowVertex
{
	// internal typedefs to simplify syntax
	typedef FHflowVertex<Object, CostType>* VertPtr;
	typedef map<VertPtr, CostType> EdgePairList;

public:
	static int nSortKey;
	static stack<int> keyStack;
	static enum { SORT_BY_DATA, SORT_BY_DIST } eSortType;
	static bool setNSortType(int whichType);
	static void pushSortKey() { keyStack.push(nSortKey); }
	static void popSortKey() { nSortKey = keyStack.top(); keyStack.pop(); }

	static int const INFINITY_FH = INT_MAX;  // defined in limits.h
	EdgePairList flowAdjList, resAdjList;
	Object data;
	CostType dist;
	VertPtr nextInPath;  // used for client-specific info

	FHflowVertex(const Object & x = Object());

	bool operator<(const FHflowVertex<Object, CostType> & rhs) const;
	const FHflowVertex<Object, CostType> & operator=
		(const FHflowVertex<Object, CostType> & rhs);
	void addToFlowAdjList(VertPtr neighbor, CostType cost);
	void addToResAdjList(VertPtr neighbor, CostType cost);
	void showFlowAdjList();
	void showResAdjList();
};

// --------------- FHflowGraph Prototype ---------------
template <class Object, typename CostType>
class FHflowGraph
{
	// internal typedefs to simplify syntax
	typedef FHflowVertex<Object, CostType> Vertex;
	typedef FHflowVertex<Object, CostType>* VertPtr;
	typedef map<VertPtr, CostType> EdgePairList;
	typedef set<VertPtr> VertPtrSet;
	typedef set<Vertex> VertexSet;

private:
	VertPtrSet vertPtrSet;
	VertexSet vertexSet;
	VertPtr startVertPtr, endVertPtr;

public:
	FHflowGraph();
	void addEdge(const Object &source, const Object &dest, CostType cost);
	VertPtr addToVertexSet(const Object & object);
	void clear();
	void showFlowAdjTable();
	void showResAdjTable();
	bool setStartVert(const Object &x);
	bool setEndVert(const Object &x);

	// algorithms
	CostType findMaxFlow();

private:
	VertPtr getVertexWithThisData(const Object & x);
	bool establishNextFlowPath();
	CostType getLimitingFlowOnResPath();
	bool adjustPathByCost(CostType cost);
	CostType getCostOfResEdge(VertPtr src, VertPtr dst);
	bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost);
	bool addCostToResEdge(VertPtr src, VertPtr dst, CostType cost);
};

// --------------- SPECS MAIN ---------------
int main()
{
	int finalFlow;

	// build graph
	FHflowGraph<string, int> myG;

	myG.addEdge("s", "a", 3);    myG.addEdge("s", "b", 2);
	myG.addEdge("a", "b", 1);    myG.addEdge("a", "c", 3); myG.addEdge("a", "d", 4);
	myG.addEdge("b", "d", 2);
	myG.addEdge("c", "t", 2);
	myG.addEdge("d", "t", 3);

	// show the original flow graph
	myG.showResAdjTable();
	myG.showFlowAdjTable();

	myG.setStartVert("s");
	myG.setEndVert("t");
	finalFlow = myG.findMaxFlow();

	cout << "Final flow: " << finalFlow << endl;

	myG.showResAdjTable();
	myG.showFlowAdjTable();

	return 0;
}

/*// --------------- MODULES MAIN ---------------
int main()
{
	// build graph
	FHflowGraph<string, int> myGraph1;
	myGraph1.addEdge("v1", "v2", 2);    myGraph1.addEdge("v1", "v4", 1);
	myGraph1.addEdge("v2", "v4", 3);    myGraph1.addEdge("v2", "v5", 10);
	myGraph1.addEdge("v3", "v1", 4);    myGraph1.addEdge("v3", "v6", 5);
	myGraph1.addEdge("v4", "v3", 2);    myGraph1.addEdge("v4", "v5", 2);
	myGraph1.addEdge("v4", "v6", 8);    myGraph1.addEdge("v4", "v7", 4);
	myGraph1.addEdge("v5", "v7", 6);
	myGraph1.addEdge("v7", "v6", 1);

	// for debugging - only works if cout << overloaded for data type
	myGraph1.showResAdjTable();
	myGraph1.showFlowAdjTable();

	// dijkstra called from inside
	//myGraph1.showDistancesTo("v2");
	cout << endl;
	int finalFlow;

	myGraph1.setStartVert("v2");
	myGraph1.setEndVert("v6");
	cout << "Using v2 as startVert, and v6 as endVert.\n";
	finalFlow = myGraph1.findMaxFlow();
	cout << "Final flow: " << finalFlow << endl;
	myGraph1.showResAdjTable();
	myGraph1.showFlowAdjTable();
}*/

/* --------------- MY MAIN ---------------
int main()
{
	// build graph
	FHflowGraph<string, int> myGraph1;
	myGraph1.addEdge("v1", "v2", 1);    myGraph1.addEdge("v1", "v3", 2);
	myGraph1.addEdge("v2", "v3", 3);    myGraph1.addEdge("v2", "v4", 4);
	myGraph1.addEdge("v3", "v4", 5);    myGraph1.addEdge("v3", "v5", 6);
	myGraph1.addEdge("v4", "v5", 7);    myGraph1.addEdge("v4", "v8", 8);
	myGraph1.addEdge("v5", "v6", 9);    myGraph1.addEdge("v5", "v7", 10);
	myGraph1.addEdge("v6", "v7", 11);    myGraph1.addEdge("v6", "v8", 12);


	// for debugging - only works if cout << overloaded for data type
	myGraph1.showResAdjTable();
	myGraph1.showFlowAdjTable();

	// dijkstra called from inside
	//myGraph1.showDistancesTo("v2");
	cout << endl;
	int finalFlow;

	myGraph1.setStartVert("v1");
	myGraph1.setEndVert("v8");
	cout << "Using v1 as startVert, and v8 as endVert.\n";
	finalFlow = myGraph1.findMaxFlow();
	cout << "Final flow: " << finalFlow << endl;
	myGraph1.showResAdjTable();
	myGraph1.showFlowAdjTable();
}*/

// --------------- FHflowVertex Method Definitions ---------------
// static const initializations for Vertex --------------
template <class Object, typename CostType>
int FHflowVertex<Object, CostType>::nSortKey
= FHflowVertex<Object, CostType>::SORT_BY_DATA;

template <class Object, typename CostType>
stack<int> FHflowVertex<Object, CostType>::keyStack;
// ------------------------------------------------------

template <class Object, typename CostType>
bool FHflowVertex<Object, CostType>::setNSortType(int whichType)
{
	switch (whichType)
	{
	case SORT_BY_DATA:
	case SORT_BY_DIST:
		nSortKey = whichType;
		return true;
	default:
		return false;
	}
}

template <class Object, typename CostType>
FHflowVertex<Object, CostType>::FHflowVertex(const Object & x)
	: data(x), dist((CostType)INFINITY_FH),
	nextInPath(NULL)
{
	// nothing to do
}

template <class Object, typename CostType>
bool FHflowVertex<Object, CostType>::operator<(
	const FHflowVertex<Object, CostType> & rhs) const
{
	switch (nSortKey)
	{
	case SORT_BY_DIST:
		return (dist < rhs.dist);
	case SORT_BY_DATA:
		return (data < rhs.data);
	default:
		return false;
	}
}

template <class Object, typename CostType>
const FHflowVertex<Object, CostType> & FHflowVertex<Object, CostType>::operator=(
	const FHflowVertex<Object, CostType> & rhs)
{
	flowAdjList = rhs.flowAdjList;
	resAdjList = rhs.resAdjList;
	data = rhs.data;
	dist = rhs.dist;
	nextInPath = rhs.nextInPath;;
	return *this;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::addToFlowAdjList
(FHflowVertex<Object, CostType> *neighbor, CostType cost)
{
	flowAdjList[neighbor] = cost;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::addToResAdjList
(FHflowVertex<Object, CostType> *neighbor, CostType cost)
{
	resAdjList[neighbor] = cost;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::showFlowAdjList()
{
	typename EdgePairList::iterator iter;

	cout << "FlowAdj List for " << data << ": ";
	for (iter = flowAdjList.begin(); iter != flowAdjList.end(); ++iter)
		cout << iter->first->data << "(" << iter->second << ") ";
	cout << endl;
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::showResAdjList()
{
	typename EdgePairList::iterator iter;

	cout << "ResAdj List for " << data << ": ";
	for (iter = resAdjList.begin(); iter != resAdjList.end(); ++iter)
		cout << iter->first->data << "(" << iter->second << ") ";
	cout << endl;
}

// --------------- FHflowGraph Method Definitions ---------------
template <class Object, typename CostType>
FHflowGraph<Object, CostType>::FHflowGraph()
{
	startVertPtr = nullptr;
	endVertPtr = nullptr;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::addEdge(
	const Object &source, const Object &dest, CostType cost)
{
	VertPtr src, dst;

	// put both source and dest into vertex list(s) if not already there
	src = addToVertexSet(source);
	dst = addToVertexSet(dest);

	// add dest to source's adjacency list
	src->addToResAdjList(dst, cost);
	// Reverse edge.
	dst->addToResAdjList(src, 0);
	src->addToFlowAdjList(dst, 0);
}

template <class Object, typename CostType>
FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::addToVertexSet(
	const Object & object)
{
	pair<typename VertexSet::iterator, bool> retVal;
	VertPtr vPtr;

	// save sort key for client
	Vertex::pushSortKey();
	Vertex::setNSortType(Vertex::SORT_BY_DATA);

	// build and insert vertex into master list
	retVal = vertexSet.insert(Vertex(object));

	// get pointer to this vertex and put into vert pointer list
	vPtr = (VertPtr)&*retVal.first;
	vertPtrSet.insert(vPtr);

	Vertex::popSortKey();  // restore client sort key
	return vPtr;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::clear()
{
	vertexSet.clear();
	vertPtrSet.clear();
	startVertPtr = nullptr;
	endVertPtr = nullptr;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::showFlowAdjTable()
{
	typename VertPtrSet::iterator iter;

	cout << "------------ Flow Adjacency Table ------------ \n";
	for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
		(*iter)->showFlowAdjList();
	cout << endl;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::showResAdjTable()
{
	typename VertPtrSet::iterator iter;

	cout << "------------ Residual Adjacency Table ------------ \n";
	for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
		(*iter)->showResAdjList();
	cout << endl;
}

// Based on dijkstra()'s definition.
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::setStartVert(const Object &x)
{
	if (!getVertexWithThisData(x))
		return false;
	startVertPtr = getVertexWithThisData(x);
	return true;
}

// Based on dijkstra()'s definition.
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::setEndVert(const Object &x)
{
	if (!getVertexWithThisData(x))
		return false;
	endVertPtr = getVertexWithThisData(x);
	return true;
}

template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::findMaxFlow()
{
	while (establishNextFlowPath())
		adjustPathByCost(getLimitingFlowOnResPath());
	CostType totalFlow = NULL;
	typename EdgePairList::iterator iter;
	for (iter = startVertPtr->flowAdjList.begin();
		iter != startVertPtr->flowAdjList.end();
		++iter)
		totalFlow += iter->second;
	return totalFlow;
}

template <class Object, typename CostType>
FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::getVertexWithThisData(
	const Object & x)
{
	typename VertexSet::iterator findResult;
	Vertex vert(x);

	Vertex::pushSortKey();  // save client sort key
	Vertex::setNSortType(Vertex::SORT_BY_DATA);
	findResult = vertexSet.find(vert);
	Vertex::popSortKey();  // restore client value

	if (findResult == vertexSet.end())
		return NULL;
	return  (VertPtr)&*findResult;     // the address of the vertex in the set of originals
}

// Based on dijkstra()'s definition.
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::establishNextFlowPath()
{
	typename VertPtrSet::iterator vIter;
	typename EdgePairList::iterator edgePrIter;
	VertPtr wPtr, vPtr;
	CostType costVW;
	queue<VertPtr> partiallyProcessedVerts;

	if (startVertPtr == NULL)
		return false;

	// initialize the vertex list and place the starting vert in p_p_v queue
	for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
	{
		(*vIter)->dist = Vertex::INFINITY_FH;
		(*vIter)->nextInPath = NULL;
	}

	startVertPtr->dist = 0;
	partiallyProcessedVerts.push(startVertPtr);

	// outer dijkstra loop
	while (!partiallyProcessedVerts.empty())
	{
		vPtr = partiallyProcessedVerts.front();
		partiallyProcessedVerts.pop();

		// inner dijkstra loop: for each vert adj to v, lower its dist to s if you can
		for (edgePrIter = vPtr->resAdjList.begin();
			edgePrIter != vPtr->resAdjList.end();
			edgePrIter++)
		{
			wPtr = edgePrIter->first;
			costVW = edgePrIter->second;
			if (vPtr->dist + costVW < wPtr->dist && costVW != 0)
			{
				wPtr->dist = vPtr->dist + costVW;
				wPtr->nextInPath = vPtr;

				if (wPtr == endVertPtr)
					return true;

				// *wPtr now has improved distance, so add wPtr to p_p_v queue
				partiallyProcessedVerts.push(wPtr);
			}
		}
	}
	return false;
}

// Based on showShortestPath()'s definition.
template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::getLimitingFlowOnResPath()
{
	if (!startVertPtr || !endVertPtr || !endVertPtr->nextInPath)
		return NULL;
	CostType limitingFlow = getCostOfResEdge(endVertPtr->nextInPath, endVertPtr);
	CostType currLimiting;
	VertPtr vp;
	for (vp = endVertPtr->nextInPath; vp != startVertPtr; vp = vp->nextInPath)
	{
		if (!vp->nextInPath)
			return NULL;
		currLimiting = getCostOfResEdge(vp->nextInPath, vp);
		if (limitingFlow > currLimiting)
			limitingFlow = currLimiting;
	}
	return limitingFlow;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::adjustPathByCost(CostType cost)
{
	if (!cost || !startVertPtr || !endVertPtr)
		return false;
	VertPtr vp;
	for (vp = endVertPtr; vp != startVertPtr; vp = vp->nextInPath)
	{
		if (!vp->nextInPath)
			return false;
		addCostToResEdge(vp->nextInPath, vp, -cost);
		if (!addCostToResEdge(vp, vp->nextInPath, cost)
			|| !addCostToFlowEdge(vp->nextInPath, vp, cost))
			return false;
	}
	return true;
}

template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::getCostOfResEdge(
	VertPtr src, VertPtr dst)
{
	if (!src || !dst)
		return NULL;
	// Return whatever cost is found, even if null.
	return src->resAdjList[dst];
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::addCostToResEdge(
	VertPtr src, VertPtr dst, CostType cost)
{
	if (!src || !dst || !cost)
		return false;
	src->resAdjList[dst] += cost;
	// If null even after adding the cost, return false.
	if (!src->resAdjList[dst])
		return false;
	return true;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::addCostToFlowEdge(
	VertPtr src, VertPtr dst, CostType cost)
{
	if (!src || !dst || !cost)
		return false;
	// Using map's find() and end().
	if (src->flowAdjList.find(dst) == src->flowAdjList.end())
	{
		src->flowAdjList[dst] -= cost;
		return true;
	}
	src->flowAdjList[dst] += cost;
	// If null even after adding the cost, return false.
	if (!src->flowAdjList[dst])
		return false;
	return true;
}

#endif

/* --------- output -----------

// --------------- SPECS MAIN ---------------

------------ Residual Adjacency Table ------------
ResAdj List for a: s(0) b(1) c(3) d(4)
ResAdj List for s: a(3) b(2)
ResAdj List for b: a(0) s(0) d(2)
ResAdj List for c: a(0) t(2)
ResAdj List for d: a(0) b(0) t(3)
ResAdj List for t: c(0) d(0)

------------ Flow Adjacency Table ------------
FlowAdj List for a: b(0) c(0) d(0)
FlowAdj List for s: a(0) b(0)
FlowAdj List for b: d(0)
FlowAdj List for c: t(0)
FlowAdj List for d: t(0)
FlowAdj List for t:

Final flow: 5
------------ Residual Adjacency Table ------------
ResAdj List for a: s(3) b(1) c(1) d(3)
ResAdj List for s: a(0) b(0)
ResAdj List for b: a(0) s(2) d(0)
ResAdj List for c: a(2) t(0)
ResAdj List for d: a(1) b(2) t(0)
ResAdj List for t: c(2) d(3)

------------ Flow Adjacency Table ------------
FlowAdj List for a: b(0) c(2) d(1)
FlowAdj List for s: a(3) b(2)
FlowAdj List for b: d(2)
FlowAdj List for c: t(2)
FlowAdj List for d: t(3)
FlowAdj List for t:

// --------------- MODULES MAIN ---------------

------------ Residual Adjacency Table ------------
ResAdj List for v1: v2(2) v4(1) v3(0)
ResAdj List for v2: v1(0) v4(3) v5(10)
ResAdj List for v4: v1(0) v2(0) v5(2) v3(2) v6(8) v7(4)
ResAdj List for v5: v2(0) v4(0) v7(6)
ResAdj List for v3: v1(4) v4(0) v6(5)
ResAdj List for v6: v4(0) v3(0) v7(0)
ResAdj List for v7: v4(0) v5(0) v6(1)

------------ Flow Adjacency Table ------------
FlowAdj List for v1: v2(0) v4(0)
FlowAdj List for v2: v4(0) v5(0)
FlowAdj List for v4: v5(0) v3(0) v6(0) v7(0)
FlowAdj List for v5: v7(0)
FlowAdj List for v3: v1(0) v6(0)
FlowAdj List for v6:
FlowAdj List for v7: v6(0)


Using v2 as startVert, and v6 as endVert.
Final flow: 4
------------ Residual Adjacency Table ------------
ResAdj List for v1: v2(2) v4(1) v3(0)
ResAdj List for v2: v1(0) v4(0) v5(9)
ResAdj List for v4: v1(0) v2(3) v5(2) v3(2) v6(5) v7(4)
ResAdj List for v5: v2(1) v4(0) v7(5)
ResAdj List for v3: v1(4) v4(0) v6(5)
ResAdj List for v6: v4(3) v3(0) v7(1)
ResAdj List for v7: v4(0) v5(1) v6(0)

------------ Flow Adjacency Table ------------
FlowAdj List for v1: v2(0) v4(0)
FlowAdj List for v2: v4(3) v5(1)
FlowAdj List for v4: v5(0) v3(0) v6(3) v7(0)
FlowAdj List for v5: v7(1)
FlowAdj List for v3: v1(0) v6(0)
FlowAdj List for v6:
FlowAdj List for v7: v6(1)


// --------------- MY MAIN ---------------

------------ Residual Adjacency Table ------------
ResAdj List for v2: v1(0) v3(3) v4(4)
ResAdj List for v1: v2(1) v3(2)
ResAdj List for v3: v2(0) v1(0) v4(5) v5(6)
ResAdj List for v4: v2(0) v3(0) v5(7) v8(8)
ResAdj List for v5: v3(0) v4(0) v6(9) v7(10)
ResAdj List for v8: v4(0) v6(0)
ResAdj List for v6: v5(0) v8(12) v7(11)
ResAdj List for v7: v5(0) v6(0)

------------ Flow Adjacency Table ------------
FlowAdj List for v2: v3(0) v4(0)
FlowAdj List for v1: v2(0) v3(0)
FlowAdj List for v3: v4(0) v5(0)
FlowAdj List for v4: v5(0) v8(0)
FlowAdj List for v5: v6(0) v7(0)
FlowAdj List for v8:
FlowAdj List for v6: v8(0) v7(0)
FlowAdj List for v7:


Using v1 as startVert, and v8 as endVert.
Final flow: 3
------------ Residual Adjacency Table ------------
ResAdj List for v2: v1(1) v3(3) v4(3)
ResAdj List for v1: v2(0) v3(0)
ResAdj List for v3: v2(0) v1(2) v4(3) v5(6)
ResAdj List for v4: v2(1) v3(2) v5(7) v8(5)
ResAdj List for v5: v3(0) v4(0) v6(9) v7(10)
ResAdj List for v8: v4(3) v6(0)
ResAdj List for v6: v5(0) v8(12) v7(11)
ResAdj List for v7: v5(0) v6(0)

------------ Flow Adjacency Table ------------
FlowAdj List for v2: v3(0) v4(1)
FlowAdj List for v1: v2(1) v3(2)
FlowAdj List for v3: v4(2) v5(0)
FlowAdj List for v4: v5(0) v8(3)
FlowAdj List for v5: v6(0) v7(0)
FlowAdj List for v8:
FlowAdj List for v6: v8(0) v7(0)
FlowAdj List for v7:

----------------------------- */