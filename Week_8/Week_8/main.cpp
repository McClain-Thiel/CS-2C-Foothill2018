//#include "Header.h"
//#include "Queue.h"
#include "Queue.h"

int main(int argc, const char * argv[]) {
   Queue myQueue;
   myQueue.enque(1);
   myQueue.enque(2);
   myQueue.enque(3);
   for(int i = 0; i < 3; i++)
      cout << myQueue.dequeue() << endl;
   cout << myQueue.dequeue(); // test deque from empty Queue (returns false)
   cout << myQueue.dequeue();
}

/*==============output=============
 1
 2
 3
 00Program ended with exit code: 0
==================================*/
