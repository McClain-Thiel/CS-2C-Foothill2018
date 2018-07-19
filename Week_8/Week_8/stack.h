//Anton_Thiel extra credit week 8

#include <iostream>
using namespace std;

struct Node {
   int data;
   Node *next;
};

class Stack {
private:
   Node* head;
   Node* tail;
   void deleteAllNodes();
public:
   Stack();
   ~Stack();
   Stack& operator=(const Stack& rhs);
   Stack(const Stack& listToCopy);
   void push(int data);
   int pop();
  };

Stack::Stack() {
   head = nullptr;
   tail = nullptr;
}
Stack::~Stack() {
   deleteAllNodes();
}
void Stack::deleteAllNodes() {
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
Stack& Stack::operator=(const Stack& rhs) {
   if (head != rhs.head) {
      deleteAllNodes();
      Node* rhsCurr = rhs.head;
      while (rhsCurr != nullptr) {
         push(rhsCurr->data);
         rhsCurr = rhsCurr -> next;
      }
   }
   return *this;
}
Stack::Stack(const Stack& listToCopy) {
   //  deleteAllNodes is safe for empty lists
   head = nullptr;
   tail = nullptr;
   *this = listToCopy;
}
void Stack::push(int data) {
   bool firstNode = false;
   if (head==nullptr)
      firstNode = true;
   Node* newNode = new Node;
   newNode->data = data;
   newNode->next = head;
   head = newNode;
   if (firstNode)
      tail = head;
}
int Stack::pop() {
   int tmpInt;
   if (head==nullptr)
      return false;
   Node* tmp;
   tmp = head;
   head = head -> next;
   if (tmp==tail) // length is one Node
      tail = nullptr;
   tmpInt = tmp->data;
   delete tmp;
   return tmpInt;

}


