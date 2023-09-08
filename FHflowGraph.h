//#ifndef FHGRAPH_H
//#define FHGRAPH_H
//#include <limits.h>
//#include <utility>
//#include <vector>
//#include <queue>
//#include <set>
//#include <map>
//#include <stack>
//#include <iostream>
//#include <functional>
//using namespace std;
//
//// CostType is some numeric type that expresses cost of edges
//// Object is the non-graph data for a vertex
//template <class Object, typename CostType>
//class FHflowVertex
//{
//	// internal typedefs to simplify syntax
//	typedef FHflowVertex<Object, CostType>* VertPtr;
//	typedef map<VertPtr, CostType> EdgePairList;
//
//public:
//	static int nSortKey;
//	static stack<int> keyStack;
//	static enum { SORT_BY_DATA, SORT_BY_DIST } eSortType;
//	static bool setNSortType(int whichType);
//	static void pushSortKey() { keyStack.push(nSortKey); }
//	static void popSortKey() { nSortKey = keyStack.top(); keyStack.pop(); }
//
//	static int const INFINITY_FH = INT_MAX;  // defined in limits.h
//	EdgePairList flowAdjList, resAdjList;
//	Object data;
//	CostType dist;
//	VertPtr nextInPath;  // used for client-specific info
//
//	FHflowVertex(const Object & x = Object());
//
//	bool operator<(const FHflowVertex<Object, CostType> & rhs) const;
//	const FHflowVertex<Object, CostType> & operator=
//		(const FHflowVertex<Object, CostType> & rhs);
//	void addToFlowAdjList(VertPtr neighbor, CostType cost);
//	void addToResAdjList(VertPtr neighbor, CostType cost);
//	void showFlowAdjList();
//	void showResAdjList();
//};
//
//// static const initializations for Vertex --------------
//template <class Object, typename CostType>
//int FHflowVertex<Object, CostType>::nSortKey
//= FHflowVertex<Object, CostType>::SORT_BY_DATA;
//
//template <class Object, typename CostType>
//stack<int> FHflowVertex<Object, CostType>::keyStack;
//// ------------------------------------------------------
//
//template <class Object, typename CostType>
//bool FHflowVertex<Object, CostType>::setNSortType(int whichType)
//{
//	switch (whichType)
//	{
//	case SORT_BY_DATA:
//	case SORT_BY_DIST:
//		nSortKey = whichType;
//		return true;
//	default:
//		return false;
//	}
//}
//
//template <class Object, typename CostType>
//FHflowVertex<Object, CostType>::FHflowVertex(const Object & x)
//	: data(x), dist((CostType)INFINITY_FH),
//	nextInPath(NULL)
//{
//	// nothing to do
//}
//
//template <class Object, typename CostType>
//bool FHflowVertex<Object, CostType>::operator<(
//	const FHflowVertex<Object, CostType> & rhs) const
//{
//	switch (nSortKey)
//	{
//	case SORT_BY_DIST:
//		return (dist < rhs.dist);
//	case SORT_BY_DATA:
//		return (data < rhs.data);
//	default:
//		return false;
//	}
//}
//
//template <class Object, typename CostType>
//const FHflowVertex<Object, CostType> & FHflowVertex<Object, CostType>::operator=(
//	const FHflowVertex<Object, CostType> & rhs)
//{
//	flowAdjList = rhs.flowAdjList;
//	resAdjList = rhs.resAdjList;
//	data = rhs.data;
//	dist = rhs.dist;
//	nextInPath = rhs.nextInPath;;
//	return *this;
//}
//
//template <class Object, typename CostType>
//void FHflowVertex<Object, CostType>::addToFlowAdjList
//(FHflowVertex<Object, CostType> *neighbor, CostType cost)
//{
//	flowAdjList[neighbor] = cost;
//}
//
//template <class Object, typename CostType>
//void FHflowVertex<Object, CostType>::addToResAdjList
//(FHflowVertex<Object, CostType> *neighbor, CostType cost)
//{
//	resAdjList[neighbor] = cost;
//}
//
//template <class Object, typename CostType>
//void FHflowVertex<Object, CostType>::showFlowAdjList()
//{
//	typename EdgePairList::iterator iter;
//
//	cout << "FlowAdj List for " << data << ": ";
//	for (iter = flowAdjList.begin(); iter != flowAdjList.end(); ++iter)
//		cout << iter->first->data << "(" << iter->second << ") ";
//	cout << endl;
//}
//
//template <class Object, typename CostType>
//void FHflowVertex<Object, CostType>::showResAdjList()
//{
//	typename EdgePairList::iterator iter;
//
//	cout << "ResAdj List for " << data << ": ";
//	for (iter = resAdjList.begin(); iter != resAdjList.end(); ++iter)
//		cout << iter->first->data << "(" << iter->second << ") ";
//	cout << endl;
//}
//
//template <class Object, typename CostType>
//class FHflowGraph
//{
//	// internal typedefs to simplify syntax
//	typedef FHflowVertex<Object, CostType> Vertex;
//	typedef FHflowVertex<Object, CostType>* VertPtr;
//	typedef map<VertPtr, CostType> EdgePairList;
//	typedef set<VertPtr> VertPtrSet;
//	typedef set<Vertex> VertexSet;
//
//private:
//	VertPtrSet vertPtrSet;
//	VertexSet vertexSet;
//	VertPtr startVertPtr, endVertPtr;
//
//public:
//	FHflowGraph();
//	void addEdge(const Object &source, const Object &dest, CostType cost);
//	VertPtr addToVertexSet(const Object & object);
//	void clear();
//	void showFlowAdjTable();
//	void showResAdjTable();
//	bool setStartVert(const Object &x);
//	bool setEndVert(const Object &x);
//
//	// algorithms
//	CostType findMaxFlow();
//
//private:
//	VertPtr getVertexWithThisData(const Object & x);
//	bool establishNextFlowPath();
//	CostType getLimitingFlowOnResPath();
//	bool adjustPathByCost(CostType cost);
//	CostType getCostOfResEdge(VertPtr src, VertPtr dst);
//	bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost);
//	bool addCostToResEdge(VertPtr src, VertPtr dst, CostType cost);
//};
//
//template <class Object, typename CostType>
//FHflowGraph<Object, CostType>::FHflowGraph()
//{
//	startVertPtr = nullptr;
//	endVertPtr = nullptr;
//}
//
//template <class Object, typename CostType>
//void FHflowGraph<Object, CostType>::addEdge(
//	const Object &source, const Object &dest, CostType cost)
//{
//	VertPtr src, dst;
//
//	// put both source and dest into vertex list(s) if not already there
//	src = addToVertexSet(source);
//	dst = addToVertexSet(dest);
//
//	// add dest to source's adjacency list
//	src->addToResAdjList(dst, cost);
//	// Reverse edge.
//	dst->addToResAdjList(src, 0);
//	src->addToFlowAdjList(dst, 0);
//}
//
//template <class Object, typename CostType>
//FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::addToVertexSet(
//	const Object & object)
//{
//	pair<typename VertexSet::iterator, bool> retVal;
//	VertPtr vPtr;
//
//	// save sort key for client
//	Vertex::pushSortKey();
//	Vertex::setNSortType(Vertex::SORT_BY_DATA);
//
//	// build and insert vertex into master list
//	retVal = vertexSet.insert(Vertex(object));
//
//	// get pointer to this vertex and put into vert pointer list
//	vPtr = (VertPtr)&*retVal.first;
//	vertPtrSet.insert(vPtr);
//
//	Vertex::popSortKey();  // restore client sort key
//	return vPtr;
//}
//
//template <class Object, typename CostType>
//void FHflowGraph<Object, CostType>::clear()
//{
//	vertexSet.clear();
//	vertPtrSet.clear();
//	startVertPtr = nullptr;
//	endVertPtr = nullptr;
//}
//
//template <class Object, typename CostType>
//void FHflowGraph<Object, CostType>::showFlowAdjTable()
//{
//	typename VertPtrSet::iterator iter;
//
//	cout << "------------------------ \n";
//   for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
//      (*iter)->showFlowAdjList();
//	cout << endl;
//}
//
//template <class Object, typename CostType>
//void FHflowGraph<Object, CostType>::showResAdjTable()
//{
//	typename VertPtrSet::iterator iter;
//
//	cout << "------------------------ \n";
//	for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
//		(*iter)->showResAdjList();
//	cout << endl;
//}
//
//// Based on dijkstra()'s definition.
//template <class Object, typename CostType>
//bool FHflowGraph<Object, CostType>::setStartVert(const Object &x)
//{
//	startVertPtr = getVertexWithThisData(x);
//	if (!startVertPtr)
//		return false;
//	return true;
//}
//
//// Based on dijkstra()'s definition.
//template <class Object, typename CostType>
//bool FHflowGraph<Object, CostType>::setEndVert(const Object &x)
//{
//	endVertPtr = getVertexWithThisData(x);
//	if (!endVertPtr)
//		return false;
//	return true;
//}
//
//template <class Object, typename CostType>
//CostType FHflowGraph<Object, CostType>::findMaxFlow()
//{
//	CostType limitResFlow;
//	while (establishNextFlowPath())
//	{
//		limitResFlow = getLimitingFlowOnResPath();
//		adjustPathByCost(limitResFlow);
//	}
//
//	CostType totalFlow = NULL;
//	typename EdgePairList::iterator iter;
//
//	for (iter = startVertPtr->flowAdjList.begin();
//		iter != startVertPtr->flowAdjList.end(); ++iter)
//		totalFlow += iter->second;
//
//	return totalFlow;
//
//}
//
//template <class Object, typename CostType>
//FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::getVertexWithThisData(
//	const Object & x)
//{
//	typename VertexSet::iterator findResult;
//	Vertex vert(x);
//
//	Vertex::pushSortKey();  // save client sort key
//	Vertex::setNSortType(Vertex::SORT_BY_DATA);
//	findResult = vertexSet.find(vert);
//	Vertex::popSortKey();  // restore client value
//
//	if (findResult == vertexSet.end())
//		return NULL;
//	return  (VertPtr)&*findResult;     // the address of the vertex in the set of originals
//}
//
//// Based on dijkstra()'s definition.
//template <class Object, typename CostType>
//bool FHflowGraph<Object, CostType>::establishNextFlowPath()
//{
//	typename VertPtrSet::iterator vIter;
//	typename EdgePairList::iterator edgePrIter;
//	VertPtr wPtr, vPtr;
//	CostType costVW;
//	queue<VertPtr> partiallyProcessedVerts;
//
//	if (startVertPtr == NULL)
//		return false;
//
//	// initialize the vertex list and place the starting vert in p_p_v queue
//	for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
//	{
//		(*vIter)->dist = Vertex::INFINITY_FH;
//		(*vIter)->nextInPath = NULL;
//	}
//
//	startVertPtr->dist = 0;
//	partiallyProcessedVerts.push(startVertPtr);
//
//	// outer dijkstra loop
//	while (!partiallyProcessedVerts.empty())
//	{
//		vPtr = partiallyProcessedVerts.front();
//		partiallyProcessedVerts.pop();
//
//      // inner dijkstra loop: for each vert adj to v, lower its dist to s if you can
//		for (edgePrIter = vPtr->resAdjList.begin();
//			edgePrIter != vPtr->resAdjList.end();
//			edgePrIter++)
//		{
//			wPtr = edgePrIter->first;
//			costVW = edgePrIter->second;
//			if (vPtr->dist + costVW < wPtr->dist && costVW != 0)
//			{
//				wPtr->dist = vPtr->dist + costVW;
//				wPtr->nextInPath = vPtr;
//
//				if (wPtr == endVertPtr)
//					return true;
//
//				// *wPtr now has improved distance, so add wPtr to p_p_v queue
//				partiallyProcessedVerts.push(wPtr);
//			}
//		}
//	}
//	return false;
//}
//
//// Based on showShortestPath()'s definition.
//template <class Object, typename CostType>
//CostType FHflowGraph<Object, CostType>::getLimitingFlowOnResPath()
//{
//   if (!startVertPtr || !endVertPtr || !endVertPtr->nextInPath)
//      return NULL;
//	CostType limitingFlow = getCostOfResEdge(endVertPtr->nextInPath, endVertPtr);
//	CostType currLimiting;
//	VertPtr vp;
//   for (vp = endVertPtr->nextInPath; vp != startVertPtr; vp = vp->nextInPath)
//   {
//      if (!vp->nextInPath)
//         return NULL;
//		currLimiting = getCostOfResEdge(vp->nextInPath, vp);
//      if (limitingFlow > currLimiting)
//         limitingFlow = currLimiting;
//   }
//   return limitingFlow;
//}
//
//template <class Object, typename CostType>
//bool FHflowGraph<Object, CostType>::adjustPathByCost(CostType cost)
//{
//	if (!cost || !startVertPtr || !endVertPtr)
//		return false;
//	VertPtr vp;
//	for (vp = endVertPtr; vp != startVertPtr; vp = vp->nextInPath)
//	{
//		if (!vp->nextInPath)
//			return false;
//		addCostToResEdge(vp->nextInPath, vp, -cost);
//		if(!addCostToResEdge(vp, vp->nextInPath, cost)
//			|| !addCostToFlowEdge(vp->nextInPath, vp, cost))
//			return false;
//	}
//	return true;
//}
//
//template <class Object, typename CostType>
//CostType FHflowGraph<Object, CostType>::getCostOfResEdge(
//	VertPtr src, VertPtr dst)
//{
//	if (!src || !dst)
//		return NULL;
//	// Return whatever cost is found, even if null.
//	return src->resAdjList[dst];
//}
//
//template <class Object, typename CostType>
//bool FHflowGraph<Object, CostType>::addCostToResEdge(
//	VertPtr src, VertPtr dst, CostType cost)
//{
//	if (!src || !dst || !cost)
//		return false;
//	src->resAdjList[dst] += cost;
//	// If null even after adding the cost, return false.
//	if (!src->resAdjList[dst])
//		return false;
//	return true;
//}
//
//template <class Object, typename CostType>
//bool FHflowGraph<Object, CostType>::addCostToFlowEdge(
//	VertPtr src, VertPtr dst, CostType cost)
//{
//	if (!src || !dst || !cost)
//		return false;
//	// Using map's find() and end().
//	if (src->flowAdjList.find(dst) == src->flowAdjList.end())
//	{
//		src->flowAdjList[dst] -= cost;
//		return true;
//	}
//	src->flowAdjList[dst] += cost;
//	// If null even after adding the cost, return false.
//	if (!src->flowAdjList[dst])
//		return false;
//	return true;
//}
//
//#endif