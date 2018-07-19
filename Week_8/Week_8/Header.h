//Anton Thiel CS2B Week 8 Foothill college

//Answer to question: Adding the tail to the list makes it possible to
//transverse the list without a counter. This is especcially usefull in the
//insertToEnd() function as it allows us to jump straigt to the end of the
//list and append it. It also makes the sof delete method possible for the
//RemoveAtEnd() function. Simply transversing with pointers won't work beacuse
//the the method doesn't evaluate if it's been "deleted" or not.


#include <iostream>
using namespace std;

struct Node {
   int data;
   Node *next;
   bool deleted;
};

class LinkedList {
private:
   Node* head;
   Node* tail;
   int numNodes;
   int numNodesExisting;
   void deleteAllNodes();
public:
   LinkedList();
   ~LinkedList();
   LinkedList& operator=(const LinkedList& rhs);
   LinkedList(const LinkedList& listToCopy);
   void printAllNodes() const;
   void insertAtFront(int data);
   bool removeAtFront();
   // demonstrating difference between having tail pointer or not
   void insertAtEnd(int data);
   bool removeAtEnd();
   void cleanList();
   int getNumNodes() {return numNodes;};
   int getNumNodesExisting() {return numNodesExisting;};
};

LinkedList::LinkedList() {
   head = nullptr;
   tail = nullptr;
}
LinkedList::~LinkedList() {
   deleteAllNodes();
}
void LinkedList::deleteAllNodes() {
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
LinkedList& LinkedList::operator=(const LinkedList& rhs) { // i think this one still works
   if (head != rhs.head) {
      deleteAllNodes();
      Node* rhsCurr = rhs.head;
      while (rhsCurr != nullptr) {
         insertAtEnd(rhsCurr->data);
         rhsCurr = rhsCurr -> next;
      }
   }
   return *this;
}
LinkedList::LinkedList(const LinkedList& listToCopy) {
   //  deleteAllNodes is safe for empty lists
   head = nullptr;
   tail = nullptr;
   *this = listToCopy;
}
void LinkedList::insertAtFront(int data) {
   // adding tail functionality
   bool firstNode = false;
   if (head==nullptr)
      firstNode = true;
   Node* newNode = new Node;
   newNode->data = data;
   newNode->next = head;
   newNode->deleted = false;
   numNodes++;
   numNodesExisting++;
   head = newNode;
   if (firstNode)
      tail = head;
}
bool LinkedList::removeAtFront() {
   if (head==nullptr)
      return false;
   Node* tmp;
   tmp = head;
   head = head -> next;
   numNodes--;
   tmp->deleted = true;
   return true;
}

// This method makes use of tail
void LinkedList::insertAtEnd(int data) {
   Node* newNode = new Node;
   newNode->deleted = false;
   newNode->data = data;
   newNode->next = nullptr;
   if (head==nullptr) // empty list case
      head = newNode;
   if (tail!=nullptr) //
      tail->next = newNode;
   tail = newNode;
   numNodes++;
   numNodesExisting++;

}


// This method makes use of tail
bool LinkedList::removeAtEnd() { //check this
   Node* secondToLast = head;
   int i = 0;
   if(numNodes == 0) // empty list
      return false;
   else if(numNodes == 1)
   {
      head->deleted = true;
      numNodes--;
      return true;
   }
   else
   {
      while(i < numNodes-1)
      {
         if(secondToLast-> deleted == false)
            i++;
         secondToLast = secondToLast->next;
      }
      secondToLast->deleted = true;
      numNodes--;
      return true;
   }
}

void LinkedList::printAllNodes() const {
   cout << "printing list with numNodes " << numNodes <<
   " and numNodesExisting " << numNodesExisting << endl;
   Node* current = head;
   if (head!=nullptr) {
      cout << "head node is: " << head->data << endl;
      while(current != nullptr) {
         cout << current->data << "|" << current->deleted << " ";
         current = current->next;
      }
      cout << endl;
   }
   else
      cout << "head is null" << endl;
   if (tail!=nullptr)
      cout << "tail node is: " << tail->data << endl;
   else
      cout << "tail is null" << endl;
   cout << endl;
}
void LinkedList::cleanList()
{
   Node* current = head;
   Node* temp;
   for(int i = 0; i < numNodesExisting; i++)
   {
      if(current->deleted == true)
      {
         current = temp;
         delete current;
         numNodesExisting--;
         current = temp;
      }
      current = current->next;
   }
   if(tail->deleted == true) // doens't update tail but keeps numNodesExisting
      numNodesExisting--;
}

