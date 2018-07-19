// File FHflowGraph.h EXPERIMENT
// Template definitions for FHflowGraph.
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
class FHvertex
{
   // internal typedefs to simplify syntax
   typedef FHvertex<Object, CostType>* VertPtr;
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

   FHvertex( const Object & x = Object() );

   //void addToAdjList(VertPtr neighbor, CostType cost);
   void addToFlowAdjList(VertPtr neighbor, CostType cost); //check params
   void addToResAdjList(VertPtr neighbor, CostType cost);
   bool operator<( const FHvertex<Object, CostType> & rhs) const;
   const FHvertex<Object, CostType> & operator=  //trivial modification
   ( const FHvertex<Object, CostType> & rhs);
   void showFlowAdjList();
   void showResAdjList();
   //void showAdjList();
};

// static const initializations for Vertex --------------
template <class Object, typename CostType>
int FHvertex<Object, CostType>::nSortKey
= FHvertex<Object, CostType>::SORT_BY_DATA;

template <class Object, typename CostType>
stack<int> FHvertex<Object, CostType>::keyStack;
// ------------------------------------------------------

template <class Object, typename CostType>
bool FHvertex<Object, CostType>::setNSortType( int whichType )
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
FHvertex<Object, CostType>::FHvertex( const Object & x)
: data(x), dist((CostType)INFINITY_FH),
nextInPath(NULL)
{
   // nothing to do
}

//======need modifications======
template <class Object, typename CostType>
void FHvertex<Object, CostType>::addToFlowAdjList
(FHvertex<Object, CostType> *neighbor, CostType cost)
{
   flowAdjList[neighbor] = cost; //probably not right
}

template <class Object, typename CostType>
void FHvertex<Object, CostType>::addToResAdjList
(FHvertex<Object, CostType> *neighbor, CostType cost)
{
   resAdjList[neighbor] = cost; //probably not right
}
//=======================

template <class Object, typename CostType>
bool FHvertex<Object, CostType>::operator<(
                                           const FHvertex<Object, CostType> & rhs) const
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
const FHvertex<Object, CostType> & FHvertex<Object, CostType>::operator=(
                                                                         const FHvertex<Object, CostType> & rhs)
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
void FHvertex<Object, CostType>::showFlowAdjList()
{
   typename EdgePairList::iterator iter;

   cout << "Adj List for " << data << ": ";
   for (iter = flowAdjList.begin(); iter != flowAdjList.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}
template <class Object, typename CostType>
void FHvertex<Object, CostType>::showResAdjList()
{
   typename EdgePairList::iterator iter;

   cout << "Adj List for " << data << ": ";
   for (iter = resAdjList.begin(); iter != resAdjList.end(); ++iter)
      cout << iter->first->data << "(" << iter->second << ") ";
   cout << endl;
}

template <class Object, typename CostType>
class FHflowGraph
{
   // internal typedefs to simplify syntax
   typedef FHvertex<Object, CostType> Vertex;
   typedef FHvertex<Object, CostType>* VertPtr;
   typedef map<VertPtr, CostType> EdgePairList;
   typedef set<VertPtr> VertPtrSet;
   typedef set<Vertex> VertexSet;


private:
   VertPtrSet vertPtrSet;
   VertexSet vertexSet;
   VertPtr startVertPtr, endVertPtr;

public:
   FHflowGraph () {}
   void addEdge(const Object &source, const Object &dest, CostType cost); //modifyed
   VertPtr addToVertexSet(const Object & object);
   void clear();

   //new bois ==trivial
   void showFlowAdjTable();
   void showResAdjTable();
   bool setStartVert(const Object &x);
   bool setEndVert(const Object &x);

   // algorithms
   bool dijkstra(const Object & x); //basically dead
   CostType findMaxFlow(); // big boi

private:
   VertPtr getVertexWithThisData(const Object & x);

   //new helpers
   bool establishNextFlowPath(); //based on dikstra
   CostType getLimitingFlowOnResPath(); //helper for find max flow
   CostType getCostOfResEdge(VertPtr src, VertPtr dst);
   bool adjustPathByCost(CostType cost); //same as above
   bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost); //helper to adj path^
   bool addCostToResEdge(VertPtr src, VertPtr dst, CostType cost);

};

/* ==mods to this class
  ● Added:
 ○ CostType findMaxFlow()​ - the main public algorithm. (All the
 remaining algorithms are helpers and can be private.) It returns the
 maximum flow found. The method loops, calling
 establishNextFlowPath()​ followed by
 getLimitingFlowOnResPath()​ and follows those calls by adjusting the
 residual and flow graphs using adjustPathByCost()​. When
 establishNextFlowPath()​ returns false​ (or adjustPathByCost()
 returns false​ or the limiting flow becomes 0​, take your pick), the loop
 ends. Finally, the flow graph is probed to find the total flow for the
 functional return.

 ○ bool establishNextFlowPath()​ - this is based on the dijkstra()
 method. It is less demanding than dijkstra​ because any path that
 connects startVertPtr​ to endVertPtr​ will do. The simplest way to
 convert dijkstra​ to this method is:
 ■ Remove the functional parameter, since we will always
 start at startVertPtr​.
 ■ When traversing a newly popped v​'s adjacency lists, skip
 edges with costVW == 0 ​since they have been reduced to
 nothing (and are effectively no longer in the residual
 graph).
 ■ End the loop as soon as it finds a path to endVertPtr​. We
 don't care about finding other flow paths since, in this
 version, it will be looking for "shorter" paths, something
 that is not necessarily good for us here.
 ■ It returns true​ if the endVertPtr ​was successfully reached
 and false​, otherwise.
 ■ (You could further modify dijkstra​ in two more places to
 generate the maximum​ path cost rather than a
 minimum​, thinking that perhaps this will yield better flow
 results. This is not necessary, but if you do implement this,
 you must be very careful to avoid infinite loops by testing
 for (and rejecting) the inevitable cycles​ that will arise from
 the extra undo-paths. I don't recommend this approach.)
 ○ This method will create the same nextInPath​ trail that dijkstra​ did.
 That path is used in the subsequent methods to adjust vertex costs in
 each of the two graphs.

 ○ CostType getLimitingFlowOnResPath()​ - a helper for
 findMaxFlow()​ which follows on the coattails of
 establishNextFlowPath()​ and uses the data and path just created to
 find the limiting flow (minimum) of the residual path just found. The
 removed method showShortestPath()​ demonstrates how to trace the
 path from endVertPtr​ to startVertPtr​, a maneuver that is useful here.

 ○ bool adjustPathByCost(CostType cost)​ - a helper for
 findMaxFlow()​ which takes the result of
 getLimitingFlowOnResPath()​ and uses it to modify the costs of
 edges in both the residual graph​ and the flow graph​. Again, chasing
 the path from end-to-start will be the dominant action here. Because
 the path was based on an ad-hoc linked list using nextInPath​, from
 end-to-start, the two vertices in each loop pass (say, vrtPtr​ and
 vrtPtr->nextInPath​) must be used to access the correct cost on the
 correct adjacency list. That's the job of the next three methods:

 ○ CostType getCostOfResEdge(VertPtr src, VertPtr dst)​ - a helper to
 getLimitingFlowOnResPath()​, this method examines src'​s residual
 adjacency list to find dst​ and then return the cost​ of the edge (src,
 dst)​.

 ○ bool addCostToResEdge(VertPtr src, VertPtr dst, CostType cost)
 - a helper to adjustPathByCost()​, this method examines src'​s
 residual adjacency list​ to find dst​ and then add cost​ to that edge
 (cost​ can be negative if adjustPathByCost() ​wants to subtract rather
 than add) It returns true​ if there is no error in the arguments (NULL​,
 e.g.)

 ○ bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost)
 - a helper to adjustPathByCost()​, this method examines src'​s flow
 adjacency list​ to find dst​ and then adds cost​ to that edge. If dst​ is
 not found in src's​ list, that implies that the edge passed in was actually
 a reverse edge, in which case the flow edge we need to adjust is (dst,
 src)​. Further, this means we need to subtract the flow because residual
 flow in the reverse direction is a signal that we are undoing some flow
 previously added. It returns true​ if there is no error in the arguments
 (NULL​, e.g.)
 */

template <class Object, typename CostType>
FHvertex<Object, CostType>* FHflowGraph<Object, CostType>::addToVertexSet(
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


   /*
    ○ addEdge()​ - adds vertices as before, but adjacency lists are handled as
    follows:
    ■ resAdjLists​ will receive two edges based on this one call:
    a forward edge, exactly as before and a reverse edge
    with cost 0
    ■ flowAdjLists​ are built as before but with all costs = 0

    */
   // add dest to source's adjacency list
   src->addToResAdjList(dst, cost);
   src->addToResAdjList(dst, 0);
   src->addToFlowAdjList(dst, 0);
}


template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::dijkstra(const Object & x)
{
   typename VertPtrSet::iterator vIter;
   typename EdgePairList::iterator edgePrIter;
   VertPtr wPtr, sPtr, vPtr;
   CostType costVW;
   queue<VertPtr> partiallyProcessedVerts;

   sPtr = getVertexWithThisData(x);
   if (sPtr == NULL)
      return false;

   // initialize the vertex list and place the starting vert in p_p_v queue
   for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
   {
      (*vIter)->dist = Vertex::INFINITY_FH;
      (*vIter)->nextInPath = NULL;
   }

   sPtr->dist = 0;
   partiallyProcessedVerts.push( sPtr ); // or, FHbinHeap::insert(), e.g.

   // outer dijkstra loop
   while( !partiallyProcessedVerts.empty() )
   {
      vPtr = partiallyProcessedVerts.front();
      partiallyProcessedVerts.pop();

      // inner dijkstra loop: for each vert adj to v, lower its dist to s if you can
      for (edgePrIter = vPtr->flowAdjList.begin();
           edgePrIter != vPtr->flowAdjList.end();
           edgePrIter++)
      {
         wPtr = edgePrIter->first;
         costVW = edgePrIter->second;
         if ( vPtr->dist + costVW < wPtr->dist )
         {
            wPtr->dist = vPtr->dist + costVW;
            wPtr->nextInPath = vPtr;

            // *wPtr now has improved distance, so add wPtr to p_p_v queue
            partiallyProcessedVerts.push(wPtr);
         }
      }
   }
   return true;
}

template <class Object, typename CostType>
FHvertex<Object, CostType>* FHflowGraph<Object, CostType>::getVertexWithThisData(
                                                                                 const Object & x)
{
   typename VertexSet::iterator findResult;
   Vertex vert(x);

   Vertex::pushSortKey();  // save client sort key
   Vertex::setNSortType(Vertex::SORT_BY_DATA);
   findResult = vertexSet.find(vert);
   Vertex::popSortKey();  // restore client value

   if ( findResult == vertexSet.end() )
      return NULL;
   return  (VertPtr)&*findResult;     // the address of the vertex in the set of originals
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

/*
 //new bois ==trivial
 void showFlowAdjTable();
 void showResAdjTable();
 bool setStartVert(const Object &x);
 bool setEndVert(const Object &x);
 */

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
      min = getLimitingFlowOnResPath();//may need break statment
      adjustPathByCost(min);
   }

   for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
   {
      for (edgePrIter = (*vIter)->flowAdjList.begin();
           edgePrIter != (*vIter)->flowAdjList.end();
           ++edgePrIter)
      {
         totalCost += edgePrIter->second;
      }
   }

   return totalCost;

   /*
    CostType findMaxFlow()​ - the main public algorithm. (All the
    remaining algorithms are helpers and can be private.) It returns the
    maximum flow found. The method loops, calling
    establishNextFlowPath()​ followed by
    getLimitingFlowOnResPath()​ and follows those calls by adjusting the
    residual and flow graphs using adjustPathByCost()​. When
    establishNextFlowPath()​ returns false​ (or adjustPathByCost()
    returns false​ or the limiting flow becomes 0​, take your pick), the loop
    ends. Finally, the flow graph is probed to find the total flow for the
    functional return.

    */
}


//helpers
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::establishNextFlowPath()
{
   typename VertPtrSet::iterator vIter;
   typename EdgePairList::iterator edgePrIter;
   VertPtr wPtr, sPtr, vPtr;
   CostType costVW;
   queue<VertPtr> partiallyProcessedVerts;

   sPtr = startVertPtr;
   if (sPtr == NULL)
      return false;

   // initialize the vertex list and place the starting vert in p_p_v queue
   for (vIter = vertPtrSet.begin(); vIter != vertPtrSet.end(); ++vIter)
   {
      (*vIter)->dist = Vertex::INFINITY_FH;
      (*vIter)->nextInPath = NULL;
   }

   sPtr->dist = 0;
   partiallyProcessedVerts.push( sPtr ); // or, FHbinHeap::insert(), e.g.

   // outer dijkstra loop
   while( !partiallyProcessedVerts.empty() )
   {
      vPtr = partiallyProcessedVerts.front();
      partiallyProcessedVerts.pop();

      // inner dijkstra loop: for each vert adj to v, lower its dist to s if you can
      for (edgePrIter = vPtr->resAdjList.begin();
           edgePrIter != vPtr->resAdjList.end(); //res or flow?
           edgePrIter++)
      {
         wPtr = edgePrIter->first;
         costVW = edgePrIter->second;

         if ( vPtr->dist + costVW < wPtr->dist and costVW != 0 ) //and if cost > 0
         {
            wPtr->dist = vPtr->dist + costVW;
            wPtr->nextInPath = vPtr;

            // *wPtr now has improved distance, so add wPtr to p_p_v queue
            partiallyProcessedVerts.push(wPtr);
         }
         if(wPrt == endVertPtr)
            break;
      }
   }
   return true; //add false claim

   /*

    this is based on the dijkstra()
    method. It is less demanding than dijkstra​ because any path that
    connects startVertPtr​ to endVertPtr​ will do. The simplest way to
    convert dijkstra​ to this method is:
    ■ Remove the functional parameter, since we will always
    start at startVertPtr​.
    ■ When traversing a newly popped v​'s adjacency lists, skip
    edges with costVW == 0 ​since they have been reduced to
    nothing (and are effectively no longer in the residual
    graph).
    ■ End the loop as soon as it finds a path to endVertPtr​. We
    don't care about finding other flow paths since, in this
    version, it will be looking for "shorter" paths, something
    that is not necessarily good for us here.
    ■ It returns true​ if the endVertPtr ​was successfully reached
    and false​, otherwise.
    ■ (You could further modify dijkstra​ in two more places to
    generate the maximum​ path cost rather than a
    minimum​, thinking that perhaps this will yield better flow
    results. This is not necessary, but if you do implement this,
    you must be very careful to avoid infinite loops by testing
    for (and rejecting) the inevitable cycles​ that will arise from
    the extra undo-paths. I don't recommend this approach.)
    ○ This method will create the same nextInPath​ trail that dijkstra​ did.
    That path is used in the subsequent methods to adjust vertex costs in
    each of the two graphs.



    */

}
template<class Object, typename CostType>
CostType FHflowGraph<Object,CostType>::getCostOfResEdge(VertPtr src,VertPtr dst)
{
   return src->resAdjList[dst];

   /*
    CostType getCostOfResEdge(VertPtr src, VertPtr dst)​ - a helper to
    getLimitingFlowOnResPath()​, this method examines src'​s residual
    adjacency list to find dst​ and then return the cost​ of the edge (src,
    dst)​.
    */
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

      /*
    CostType getLimitingFlowOnResPath()​ - a helper for
    findMaxFlow()​ which follows on the coattails of
    establishNextFlowPath()​ and uses the data and path just created to
    find the limiting flow (minimum) of the residual path just found. The
    removed method showShortestPath()​ demonstrates how to trace the
    path from endVertPtr​ to startVertPtr​, a maneuver that is useful here.
       
       return min;

       CostType minimumCost, tempCost;
       VertPtr vp;

       minimumCost = getCostOfResEdge(endVertPtr->nextInPath, endVertPtr);
       for (vp = endVertPtr; vp != startVertPtr; vp = vp->nextInPath)
       {
       tempCost = getCostOfResEdge(vp->nextInPath, vp);
       if (tempCost < minimumCost)
       minimumCost = tempCost;
       }
       
       return minimumCost;

    */

}
template <class Object, typename CostType>
bool FHflowGraph<Object, CostType>::adjustPathByCost(CostType cost)
{

   VertPtr vp, start, stop;

   start = startVertPtr;
   stop = endVertPtr;
   if (start == NULL || stop == NULL)
      return false;

   for(vp = stop; vp != start; vp = vp->nextInPath)
   {
      addCostToResEdge(vp->nextInPath, vp, -cost);
      addCostToResEdge(vp, vp->nextInPath, cost);
      addCostToResEdge(vp->nextInPath, vp, cost);
   }

   return true;

   /*
    bool adjustPathByCost(CostType cost)​ - a helper for
    findMaxFlow()​ which takes the result of
    getLimitingFlowOnResPath()​ and uses it to modify the costs of
    edges in both the residual graph​ and the flow graph​. Again, chasing
    the path from end-to-start will be the dominant action here. Because
    the path was based on an ad-hoc linked list using nextInPath​, from
    end-to-start, the two vertices in each loop pass (say, vrtPtr​ and
    vrtPtr->nextInPath​) must be used to access the correct cost on the
    correct adjacency list. That's the job of the next three methods:

    */
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
                                     //literally adds cost
   }
   else
   {
      addCostToFlowEdge(dst, src, -cost); //reverse edge
   }

   /*
    bool addCostToFlowEdge(VertPtr src, VertPtr dst, CostType cost)
    - a helper to adjustPathByCost()​, this method examines src'​s flow
    adjacency list​ to find dst​ and then adds cost​ to that edge. If dst​ is
    not found in src's​ list, that implies that the edge passed in was actually
    a reverse edge, in which case the flow edge we need to adjust is (dst,
    src)​. Further, this means we need to subtract the flow because residual
    flow in the reverse direction is a signal that we are undoing some flow
    previously added. It returns true​ if there is no error in the arguments
    (NULL​, e.g.)
    */
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
