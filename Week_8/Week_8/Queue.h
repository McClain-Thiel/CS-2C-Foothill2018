#include <iostream>
using namespace std;

struct Node {
   int data;
   Node *next;
};

class Queue {
private:
   Node* head;
   Node* tail;
   void deleteAllNodes();
public:
   Queue();
   ~Queue();
   Queue& operator=(const Queue& rhs);
   Queue(const Queue& listToCopy);
   void printAllNodes() const;
   int dequeue();
   void enque(int data);
};

Queue::Queue() {
   head = nullptr;
   tail = nullptr;
}
Queue::~Queue() {
   deleteAllNodes();
}
void Queue::deleteAllNodes() {
   Node* curr = head;
   Node* tmp;
   while (curr != nullptr) {
      tmp = curr->next;
      delete curr;
      curr = tmp;
   }
   head = nullptr;
   tail = nullptr;
}
Queue& Queue::operator=(const Queue& rhs) {
   if (head != rhs.head) {
      deleteAllNodes();
      Node* rhsCurr = rhs.head;
      while (rhsCurr != nullptr) {
         enque(rhsCurr->data);
         rhsCurr = rhsCurr -> next;
      }
   }
   return *this;
}
Queue::Queue(const Queue& listToCopy) {
   //  deleteAllNodes is safe for empty lists
   head = nullptr;
   tail = nullptr;
   *this = listToCopy;
}

int Queue::dequeue() {
   int tmpInt;
   if (head==nullptr)
      return false;
   Node* tmp;
   tmp = head;
   head = head -> next;
   if (tmp==tail) // length is one Node
      tail = nullptr;
   tmpInt = tmp -> data;
   delete tmp;
   return tmpInt;
}

// This method makes use of tail
void Queue::enque(int data) {
   Node* newNode = new Node;
   newNode->data = data;
   newNode->next = nullptr;
   if (head==nullptr) // empty list case
      head = newNode;
   if (tail!=nullptr) //
      tail->next = newNode;
   tail = newNode;

}


