// client file --------------------------------------------------
// FHflowGraph client
// CS 2C Foothill College
#include <iostream>
#include <string>
using namespace std;
#include "FHflowGraph.h"
// --------------- main ---------------
int main()
{
   int finalFlow;
   // build graph
   FHflowGraph<string, int> myG;
   myG.addEdge("s","a", 3); myG.addEdge("s","b", 2);
   myG.addEdge("a","b", 1); myG.addEdge("a","c", 3); myG.addEdge("a","d", 4);
   myG.addEdge("b","d", 2);
   myG.addEdge("c","t", 2);
   myG.addEdge("d","t", 3);
   // show the original flow graph
   myG.showResAdjTable();
   myG.showFlowAdjTable();
   myG.setStartVert("s");
   myG.setEndVert("t");
   finalFlow = myG.findMaxFlow();
   cout << "Final flow: " << finalFlow << endl;
   myG.showResAdjTable();
   myG.showFlowAdjTable();

   //additional testing
   FHflowGraph<char, double> test;
   test.addEdge('a', 'b', 5);
   test.addEdge('a', 'c', 6);
   test.addEdge('a', 'd',7);
   test.addEdge('b', 'e', 8);
   test.addEdge('c', 'f', 9);
   test.addEdge('d', 'f', 10);
   test.addEdge('e', 'g', 11);
   test.addEdge('f', 'g', 12);

   test.showResAdjTable();
   test.showFlowAdjTable();
   test.setStartVert('a');
   test.setEndVert('g');
   finalFlow = test.findMaxFlow();
   cout << "Final flow: " << finalFlow << endl;
   test.showResAdjTable();
   test.showFlowAdjTable();
   return 0;
}
/* --------- output -----------
 my output with test main + additional testing:
 ------------------------
 Res List for s: a(3) b(2)
 Res List for a: s(0) b(1) c(3) d(4)
 Res List for b: s(0) a(0) d(2)
 Res List for c: a(0) t(2)
 Res List for d: a(0) b(0) t(3)
 Res List for t: c(0) d(0)

 ------------------------
 Flow List for s: a(0) b(0)
 Flow List for a: b(0) c(0) d(0)
 Flow List for b: d(0)
 Flow List for c: t(0)
 Flow List for d: t(0)
 Flow List for t:

 Final flow: 5
 ------------------------
 Res List for s: a(0) b(0)
 Res List for a: s(3) b(1) c(1) d(3)
 Res List for b: s(2) a(0) d(0)
 Res List for c: a(2) t(0)
 Res List for d: a(1) b(2) t(0)
 Res List for t: c(2) d(3)

 ------------------------
 Flow List for s: a(3) b(2)
 Flow List for a: b(0) c(2) d(1)
 Flow List for b: d(2)
 Flow List for c: t(2)
 Flow List for d: t(3)
 Flow List for t:

 ------------------------
 Res List for a: b(5) c(6) d(7)
 Res List for b: a(0) e(8)
 Res List for c: a(0) f(9)
 Res List for d: a(0) f(10)
 Res List for e: b(0) g(11)
 Res List for f: c(0) d(0) g(12)
 Res List for g: e(0) f(0)

 ------------------------
 Flow List for a: b(0) c(0) d(0)
 Flow List for b: e(0)
 Flow List for c: f(0)
 Flow List for d: f(0)
 Flow List for e: g(0)
 Flow List for f: g(0)
 Flow List for g:

 Final flow: 17
 ------------------------
 Res List for a: b(0) c(0) d(1)
 Res List for b: a(5) e(3)
 Res List for c: a(6) f(3)
 Res List for d: a(6) f(4)
 Res List for e: b(5) g(6)
 Res List for f: c(6) d(6) g(0)
 Res List for g: e(5) f(12)

 ------------------------
 Flow List for a: b(5) c(6) d(6)
 Flow List for b: e(5)
 Flow List for c: f(6)
 Flow List for d: f(6)
 Flow List for e: g(5)
 Flow List for f: g(12)
 Flow List for g: 
 
 Program ended with exit code: 0
 ----------------------------- */
