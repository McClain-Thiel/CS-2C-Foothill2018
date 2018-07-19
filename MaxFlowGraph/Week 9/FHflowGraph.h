
#ifndef FHflowGraph_H
#define FHflowGraph_H
#include <limits.h>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <iostream>
#include <functional>
using namespace std;

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
   static bool setNSortType( int whichType );
   static void pushSortKey() { keyStack.push(nSortKey); }
   static void popSortKey() { nSortKey = keyStack.top(); keyStack.pop(); }

   static int const INFINITY_FH = INT_MAX;  // defined in limits.h
   EdgePairList flowAdjList, resAdjList;
   Object data;
   CostType dist;
   VertPtr nextInPath;  // used for client-specific info

   FHflowVertex( const Object & x = Object() );

   //void addToAdjList(VertPtr neighbor, CostType cost);
   void addToFlowAdjList(VertPtr neighbor, CostType cost); //check params
   void addToResAdjList(VertPtr neighbor, CostType cost);
   bool operator<( const FHflowVertex<Object, CostType> & rhs) const;
   const FHflowVertex<Object, CostType> & operator=  //trivial modification
   ( const FHflowVertex<Object, CostType> & rhs);
   void showFlowAdjList();
   void showResAdjList();
   //void showAdjList();
};

// static const initializations for Vertex --------------
template <class Object, typename CostType>
int FHflowVertex<Object, CostType>::nSortKey
= FHflowVertex<Object, CostType>::SORT_BY_DATA;

template <class Object, typename CostType>
stack<int> FHflowVertex<Object, CostType>::keyStack;
// ------------------------------------------------------

template <class Object, typename CostType>
bool FHflowVertex<Object, CostType>::setNSortType( int whichType )
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
FHflowVertex<Object, CostType>::FHflowVertex( const Object & x)
: data(x), dist((CostType)INFINITY_FH),
nextInPath(NULL)
{
   // nothing to do
}

//======need modifications======
template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::addToFlowAdjList
(FHflowVertex<Object, CostType> *neighbor, CostType cost)
{
   flowAdjList[neighbor] = cost; //probably not right
}

template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::addToResAdjList
(FHflowVertex<Object, CostType> *neighbor, CostType cost)
{
   resAdjList[neighbor] = cost; //probably not right
}
//=======================

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
   //adjList = rhs.adjList;
   flowAdjList = rhs.flowAdjList;
   resAdjList = rhs.resAdjList;
   data = rhs.data;
   dist = rhs.dist;
   nextInPath = rhs.nextInPath;;
   return *this;
}

//=========just coppied but shpuld be right anyway=========
template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::showFlowAdjList()
{
   typename EdgePairList::iterator iter;

   cout << "Flow List for " << data << ": ";
   for (iter = flowAdjList.begin(); iter != flowAdjList.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}
template <class Object, typename CostType>
void FHflowVertex<Object, CostType>::showResAdjList()
{
   typename EdgePairList::iterator iter;

   cout << "Res List for " << data << ": ";
   for (iter = resAdjList.begin(); iter != resAdjList.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}

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
   VertPtr startVertPtr, endVertPtr; //new

public:
   FHflowGraph ();
   void addEdge(const Object &source, const Object &dest, CostType cost); //mod
   VertPtr addToVertexSet(const Object & object);
   void clear();

   //new =trivial
   void showFlowAdjTable();
   void showResAdjTable();
   bool setStartVert(const Object &x);
   bool setEndVert(const Object &x);

   // algorithms
   //bool dijkstra(const Object & x); //basically dead
   CostType findMaxFlow(); // main public

private:
   VertPtr getVertexWithThisData(const Object & x);

   //new helpers
   bool establishNextFlowPath(); //based on dikstra
   CostType getLimitingFlowOnResPath(); //helper for find max flow
   CostType getCostOfResEdge(VertPtr src, VertPtr dst);
   bool adjustPathByCost(CostType cost); //same as above
   bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost); //helper ^
   bool addCostToResEdge(VertPtr src, VertPtr dst, CostType cost);

};
//end of decloration

//Methods
template <class Object, typename CostType>
FHflowGraph<Object, CostType>::FHflowGraph()
: startVertPtr(NULL), endVertPtr(NULL){}


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
   retVal = vertexSet.insert( Vertex(object) );

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
}


template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::addEdge(
                     const Object &source, const Object &dest, CostType cost )
{
   VertPtr src, dst;

   // put both source and dest into vertex list(s) if not already there
   src = addToVertexSet(source);
   dst = addToVertexSet(dest);

   // add dest to source's adjacency list
   src->addToResAdjList(dst, cost);
   dst->addToResAdjList(src, 0);
   src->addToFlowAdjList(dst, 0);
}

template <class Object, typename CostType>
FHflowVertex<Object, CostType>* FHflowGraph<Object, CostType>::getVertexWithThisData
                                                            (const Object & x)
{
   typename VertexSet::iterator findResult;
   Vertex vert(x);

   Vertex::pushSortKey();  // save client sort key
   Vertex::setNSortType(Vertex::SORT_BY_DATA);
   findResult = vertexSet.find(vert);
   Vertex::popSortKey();  // restore client value

   if ( findResult == vertexSet.end() )
      return NULL;
   return  (VertPtr)&*findResult; // the address of the vertex in the set of or
}

//new trvial additions/modifications
template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::showFlowAdjTable()
{
   typename VertPtrSet::iterator iter;

   cout << "------------------------ \n";
   for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
      (*iter)->showFlowAdjList();
   cout << endl;
}

template <class Object, typename CostType>
void FHflowGraph<Object, CostType>::showResAdjTable()
{
   typename VertPtrSet::iterator iter;

   cout << "------------------------ \n";
   for (iter = vertPtrSet.begin(); iter != vertPtrSet.end(); ++iter)
      (*iter)->showResAdjList();
   cout << endl;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::setStartVert(const Object &x)
{
   startVertPtr = getVertexWithThisData(x);
   return (startVertPtr != NULL);
}
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::setEndVert(const Object &x)
{
   endVertPtr = getVertexWithThisData(x);
   return (endVertPtr != NULL);
}

//New algorithims
//public
template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::findMaxFlow()
{

   bool valid = true;
   CostType total = 0, min; //0 is valid because costtype has to be numeric
   typename VertPtrSet::iterator vIter;
   typename EdgePairList::iterator edgePrIter;

   while(valid)
   {
      valid = establishNextFlowPath();  //terminates loop if this returns false
      if(!valid)
         break;
      min = getLimitingFlowOnResPath();//may need break statment
      adjustPathByCost(min);
   }

   //this part needs to prob and add the values of everything flowing into t
   //this may also always be equivolent to everyhtong flowing out of src
   //right now this is probing and summing the who table, but i only want it to
   //sum and prob the start or end
   for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
   {
      for (edgePrIter = (*vIter)->flowAdjList.begin();
           edgePrIter != (*vIter)->flowAdjList.end();
           ++edgePrIter)
      {
         if( edgePrIter->first == endVertPtr)//checks to see if it flows into t
            total += edgePrIter->second;
      }
   }

   return total;
}


//helpers
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::establishNextFlowPath()
{
   typename VertPtrSet::iterator vIter;
   typename EdgePairList::iterator edgePrIter;
   VertPtr wPtr, vPtr;
   CostType costVW;
   queue<VertPtr> partiallyProcessedVerts;

   endVertPtr->nextInPath = NULL;

   if (startVertPtr == NULL)
      return false;

   // initialize the vertex list and place the starting vert in p_p_v queue
   for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
   {
      (*vIter)->dist = Vertex::INFINITY_FH;
      (*vIter)->nextInPath = NULL;
   }

   startVertPtr->dist = 0;
   partiallyProcessedVerts.push(startVertPtr); // or, FHbinHeap::insert(), e.g.

   // outer dijkstra loop
   while (!partiallyProcessedVerts.empty())
   {
      vPtr = partiallyProcessedVerts.front();
      partiallyProcessedVerts.pop();

      // inner dijkstra loop, iterating through the residual graph's adj list
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

            // *wPtr now has improved distance, so add wPtr to p_p_v queue
            partiallyProcessedVerts.push(wPtr);
         }

         if (wPtr == endVertPtr)
            break;
      }
   }

   return !(endVertPtr->nextInPath == NULL); // if no path exists return false
}

template<class Object, typename CostType>
CostType FHflowGraph<Object,CostType>::getCostOfResEdge(VertPtr src,VertPtr dst)
{
   return src->resAdjList[dst];
}

template <class Object, typename CostType>
CostType FHflowGraph<Object, CostType>::getLimitingFlowOnResPath()
{
   VertPtr vp, start, stop;
   CostType min, temp;
   stack<VertPtr> pathStack;

   start = startVertPtr;
   stop = endVertPtr;
   if (start == NULL || stop == NULL)
      return false;

   min = getCostOfResEdge(stop->nextInPath, stop);
   for(vp = stop; vp != start; vp = vp->nextInPath)
   {
      temp = getCostOfResEdge(vp->nextInPath, vp);
      if(temp < min)
         min = temp;
   }
   return min;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::adjustPathByCost(CostType cost)
{

   VertPtr vp;

   for (vp = endVertPtr; vp != startVertPtr; vp = vp->nextInPath)
   {
      addCostToResEdge(vp->nextInPath, vp, -cost);
      addCostToResEdge(vp, vp->nextInPath, cost);
      addCostToFlowEdge(vp->nextInPath, vp, cost);
   }
   return true;
}
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::addCostToFlowEdge(VertPtr src, VertPtr dst,
                                                      CostType cost)
{

   typename EdgePairList::iterator iter; //not really sure this should be iter

   if(src == NULL or dst == NULL)
      return false;


   iter = src->flowAdjList.find(dst);
   if(iter != src->flowAdjList.end())//test for reverse edge
   {
      src->flowAdjList[dst] += cost; // = or +
   }
   else
   {
      src->flowAdjList[dst] -= cost; //reverse edge
   }

   return true;
}

template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::addCostToResEdge(VertPtr src, VertPtr dst,
                                                     CostType cost)
{
   if(src == NULL or dst == NULL)
      return false;
   
   src->resAdjList[dst] += cost; //literally addds cost / not replace
   return true;
   
}

#endif

