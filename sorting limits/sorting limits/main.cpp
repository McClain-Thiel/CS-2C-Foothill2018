#include <iostream>
#include "FHsort.h"
#include "FHvector.h"
#include <cmath>
#include <time.h>
#include <iomanip> //for setprecition for formating

using namespace std;

template<typename type>
void quickSortPrivate(FHvector<type> & a, int left, int right, int rec_limit);

template<typename type>
void quickSortX(FHvector<type> & array, int rec_limit);

void printX(double value,int rec_lim); //formating function

static const int REC_MIN = 2;

int main() {

   srand(time(NULL));
   int MAX_SIZE, recMax = 300;
   double startTime, stopTime, elapsedTime;
   FHvector<int> fhVectorOfInts(MAX_SIZE);

   //outer loop controls number of elements being sorted
   //useful so that one desnt have to plug in manually
   for(int i = 100000; i <= 24000000; i = i * 2)
   {
      MAX_SIZE = i;
      fhVectorOfInts.resize(MAX_SIZE);
      cout << "Table for array of " << MAX_SIZE << " elements" << endl;

      for(int i = REC_MIN; i <= recMax+2; i = i + 2)
      {

   //populate vector with random values //rerandomized before every sort call
         for(int i = 0; i < MAX_SIZE; i++)
         {
            fhVectorOfInts[i] = rand();
         }
         startTime = clock();
         quickSortX(fhVectorOfInts, i);
         stopTime = clock();
         elapsedTime = double(stopTime - startTime)/(double)CLOCKS_PER_SEC;

         printX(elapsedTime, i);
      }
   }
}

template<typename type>
void quickSortPrivate(FHvector<type> & a, int left, int right, int rec_limit)
{
   if(rec_limit < REC_MIN)
      return;


   type pivot;
   int i, j;

   if( left + rec_limit <= right )
   {
      pivot = median3(a, left, right);
      for(i = left, j = right - 1; ; )
      {
         while( a[++i] < pivot )
            ;
         while( pivot < a[--j])
            ;
         if(i < j)
            mySwapFH(a[i], a[j]);
         else
            break;
      }

      mySwapFH(a[i], a[right - 1]);  // restore pivot

      // recursive calls on smaller sub-groups
      quickSort(a, left, i - 1);
      quickSort(a, i + 1, right);
   }
   else
      // non-recursive escape valve - insertionSort
      insertionSort(a, left, right);
}

template <typename type> //public version
void quickSortX( FHvector<type> & a, int rec_limit)
{
   quickSortPrivate(a, 0, a.size() - 1, rec_limit);
}

void printX(double value, int recLim) //formating and output method
{
      cout << "|" << recLim << " | "
      << setprecision(3)<<  value << " |" << endl;
}
/*===========table and comments =========


 /comments on sorting times
 For smaller vectors of numbers, the shortest times occurred at higher recursion
 limits. This is seen clearly in the sort of the 100,000 element array. A
 recursion limit of 2 gives a sort time of about .246 seconds, while a recursion
 limit of 300 gives a time of about .188 seconds. However in larger arrays, the
 optimal recursion limit is lower. It's hard to pinpoint an exact optimal
 recursion limit because of the random nature of this testing but shorter times
 seem to become more prevalent with recursion limits < 100 for relatively large
 arrays. This trend implies that the insertion sort, which is used once the
 function has reached its recursion limit, if actually faster than quickSort
 for smaller arrays.

 ===================================*/

