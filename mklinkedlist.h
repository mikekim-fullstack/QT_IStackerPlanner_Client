#ifndef MKLINKEDLIST_H
#define MKLINKEDLIST_H


// linked mkLinkedList example - using struct inside a class

#include <iostream>
#include <string>
using namespace std;

template <typename T>
class LList
{
    struct node
    {
        T value;
        node* prev;
        node* next;
        node(T value, node *prev = nullptr, node *next = nullptr)
            : value{std::move(value)}, prev{prev}, next{next}
        {}
        node(const node&) = delete;
        void operator=(const node&) = delete;

        friend void swap(node& a, node& b) {
            using std::swap;
            swap(a.value, b.value);
            swap(a.prev, b.prev);
            swap(a.next, b.next);
        }
    };

public:
    LList() = default;
    LList(const LList<T>&);
    LList(const LList<T>&&);
    LList& operator=(LList<T>);
    ~LList();

    T const& get(std::size_t pos) const;
    T& get(std::size_t pos);

    void insert(T value, std::size_t pos);
    T remove(std::size_t pos);

    template<typename U>
    friend void swap(LList<U>&, LList<U>&);

private:
    std::size_t n_elements = 0;
    node head = { T{}, nullptr, &tail };
    node tail = { T{}, &head, nullptr };
};

template<typename T>
LList<T>::LList(const LList<T>& other)
{
    // work from back to front, so we always insert at position 0
    // (which is the cheapest)
    for (auto i = other.n_elements;  i > 0;  --i) {
        insert(other.get(i-1), 0);
    }
}

template<typename T>
LList<T>::LList(const LList<T>&& other)
{
    swap(*this, other);
}

template<typename T>
LList<T>& LList<T>::operator=(LList<T> other)
{
    swap(*this, other);
    return *this;
}

template<typename T>
LList<T>::~LList()
{
    for (auto p = head.next;  p != &tail;  ) {
        auto next = p->next;
        delete p;
        p = next;
    }
}

template<typename T>
void swap(LList<T>& a, LList<T>& b)
{
    using std::swap;
    swap(a.head, b.head);
    swap(a.tail, b.tail);
    swap(a.n_elements, b.n_elements);
}


template<typename T>
const T& LList<T>::get(std::size_t pos) const
{
    auto p = head.next;
    while (pos--)
        p = p->next;
    return p->value;
}

template<typename T>
T& LList<T>::get(std::size_t pos)
{
    auto p = head.next;
    while (pos--)
        p = p->next;
    return p->value;
}

template<typename T>
void LList<T>::insert(T value, std::size_t pos)
{
    auto p = &head;
    while (pos--)
        p = p->next;
    auto next = p->next;
    next->prev = p->next = new node(std::move(value), p, next);
    ++n_elements;
}

template<typename T>
T LList<T>::remove(std::size_t pos)
{
    auto p = head.next;
    while (pos--)
        p = p->next;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    auto value = std::move(p->value);
    delete p;
    --n_elements;
    return value;
}

class mkLinkedList
{
public:
    struct node {
        int id;
        string name;
        struct node *next;
    } *head, *tail, *ptr;

    mkLinkedList():head(nullptr),tail(nullptr){}	// constructor
    ~mkLinkedList();			// destructor

    struct mkLinkedList::node* searchName(struct mkLinkedList::node*, string);
    struct mkLinkedList::node* searchId(struct mkLinkedList::node*, int);
    struct mkLinkedList::node* initNode(string, int);

    void reverse();
    void addNode( struct mkLinkedList::node*);
    void insertNode( struct mkLinkedList::node*);
    void deleteNode( struct mkLinkedList::node*);
    void deleteList( struct mkLinkedList::node*);
    void displayList( struct mkLinkedList::node*)const ;
    void displayNode( struct mkLinkedList::node*) const;
};

mkLinkedList::~mkLinkedList() {
    node *current,*temp;
    current = head;
    temp = head;
    while(current != nullptr) {
        current = current->next;
        delete temp;
        temp = current;
    }
}

struct mkLinkedList::node* mkLinkedList::initNode(string s, int i) {
    struct node *ptr = new node;

    // error? then just return
    if( ptr == nullptr )
        return static_cast<struct node *>(nullptr);
    // assign it
    // then return pointer to ne node
    else {
        ptr->name = s ;
        ptr->id = i;
        return ptr;
    }
}

// adding to the end of mkLinkedList
void mkLinkedList::addNode( struct node *newNode )  {
    // if there is no node, put it to head
    if( head == nullptr ) {
        head = newNode;
        tail = newNode;
    }

    // link in the new_node to the tail of the mkLinkedList
    // then mark the next field as the end of the mkLinkedList
    // adjust tail to point to the last node

    tail->next = newNode;
    newNode->next = nullptr;
    tail = newNode;
}

void mkLinkedList::insertNode( struct node *newnode ) {
   struct node *temp, *prev;

   if( head == nullptr ) {                     // if an empty mkLinkedList,
       head = newnode;                      // set 'head' to it
       tail = newnode;
       head->next = nullptr;                   // set end of mkLinkedList to nullptr
       return;
   }

   temp = head;                             // start at beginning of mkLinkedList
                        // while currentname < newname
   while( temp->name < newnode->name) {	    // to be inserted then
          temp = temp->next;                // goto the next node in mkLinkedList
          if( temp == nullptr )                // don't go past end of mkLinkedList
              break;
   }
                        // set previous node before we insert
                        // first check to see if it's inserting
   if( temp == head ) {		    	    // before the first node
      newnode->next = head;                 // link next field to original mkLinkedList
      head = newnode;                       // head adjusted to new node
   }
   else {				    // it's not the first node
      prev = head;		    	    // start of the mkLinkedList,
      while( prev->next != temp ) {
          prev = prev->next;	    	    // will cycle to node before temp
      }
      prev->next = newnode;                 // insert node between prev and next
      newnode->next = temp;
      if( tail == prev )		    // if the new node is inserted at the
         tail = newnode;		    // end of the mkLinkedList the adjust 'end'
   }
}

struct mkLinkedList::node* mkLinkedList::searchName(struct node* ptr, string name) {
    while( name != ptr->name ) {
       ptr = ptr->next;
       if( ptr == nullptr )
          break;
    }
    return ptr;
}

struct mkLinkedList::node* mkLinkedList::searchId(struct node* ptr, int id) {
    while( id != ptr->id ) {
       ptr = ptr->next;
       if( ptr == nullptr )
          break;
    }
    return ptr;
}

void mkLinkedList::reverse() {
    // we need at least two nodes for the reverse to have any effect
    if(head == nullptr || head->next == nullptr) return;

    // Starting 2nd mkLinkedList as 'me' and 'head' is now 'me->next'
    // and 'head->next' is pointing to nullptr
    // So, the 3rd mkLinkedList is now 'child' of 'me'
    node *parent = head;
    node *me = head->next;
    node *child = me->next;

    // convert head to tail
    head->next = nullptr;

    // reverse pointer direction
    me->next = head;

    while(child != nullptr){
        me->next = parent;
        parent = me;
        me = child;
        child = child->next;
    }
    // when me reached the tail
    // me becomes head
    head = me;
    // the head is now pointing to the 2nd last node
    head->next = parent;
}


void mkLinkedList::deleteNode( struct mkLinkedList::node *ptr )
{
  struct node *temp, *prev;
   temp = ptr;    // node to be deleted
   prev = head;   // start of the mkLinkedList, will cycle to node before temp

   if( temp == prev ) {                    // deleting first node?
       head = head->next;                  // moves head to next node
       if( tail == temp )                  // is it end, only one node?
          tail = tail->next;               // adjust end as well
       delete temp ;                       // free up space
   }
   else {                                  // if not the first node, then
       while( prev->next != temp ) {       // move prev to the node before
           prev = prev->next;              // the one to be deleted
       }
       prev->next = temp->next;            // link previous node to next
       if( tail == temp )                  // if this was the end node,
           tail = prev;                    // then reset the end pointer
      delete temp;                         // free up space
   }
}

void mkLinkedList::deleteList( struct node *ptr )
{
   struct node *temp;

   if( head == nullptr ) return;   	// don't try to delete an empty mkLinkedList

   if( ptr == head ) {			// if we are deleting the entire mkLinkedList
       head = nullptr;			// then reset head and
       tail = nullptr;			// end to empty
   }
   else {
       temp = head;			// if it's not the entire mkLinkedList, readjust end
       while( temp->next != ptr )       // locate previous node to ptr
           temp = temp->next;
       tail = temp;                     // set end to node before ptr
   }

   while( ptr != nullptr ) {		// whilst there are still nodes to delete
      temp = ptr->next;			// record address of next node
      delete ptr;			// free this node
      ptr = temp;			// point to next node to be deleted
   }
}

void mkLinkedList::displayNode( struct mkLinkedList::node *ptr ) const
{
    cout << ptr->id << ": " << ptr->name << endl;
}

void mkLinkedList::displayList( struct mkLinkedList::node *ptr ) const
{
    if(!ptr) cout << "Nothing to display" << endl;
    while(ptr) {
        displayNode(ptr);
        ptr = ptr->next;
    }
}

int main_LinkTest()
{
    int id;
    string name;
    mkLinkedList myList;
    mkLinkedList::node* ptr;

    // add
    ptr = myList.initNode( "s1", 1 );
    myList.addNode(ptr);
    ptr = myList.initNode( "s2", 2 );
    myList.addNode(ptr);
    ptr = myList.initNode( "s3", 3 );
    myList.addNode(ptr);
    ptr = myList.initNode( "s4", 4 );
    myList.addNode(ptr);
    ptr = myList.initNode( "s5", 5 );
    myList.addNode(ptr);
    myList.displayList(myList.head);

    // delete
    name = "s2";
    ptr = myList.searchName( myList.head, name );
    if( ptr == nullptr ) {
        cout << "\nName: " << name << " not found" << endl;
    }
    else {
        cout << "\nDeleting a node ...  ";
        myList.displayNode(ptr);
        myList.deleteNode( ptr );
    }
    myList.displayList(myList.head);

    // insert
    name = "s2";
    id = 2;
    ptr = myList.initNode( name, id );
    myList.insertNode( ptr );
    cout << "\nInserting a node ...  ";
    myList.displayNode(ptr);
    myList.displayList(myList.head);

    // reverse
    cout << "\nReversing the mkLinkedList ...  \n";
    myList.reverse();
    myList.displayList(myList.head);

    // delete
    cout << "\nIn the end, deleting the mkLinkedList ...  \n";
    myList.deleteList(myList.head);
    myList.displayList(myList.head);
    return 0;
}
int main_LListTest()
{
    LList <double> mylist;
    mylist.insert(20, 0);
    mylist.insert(30, 1);
    mylist.insert(40, 2);
    mylist.insert(30, 2);
    mylist.insert(10, 0);
    std::cout << "removing: " << mylist.remove(0) << std::endl;
    mylist.insert(10, 0);
    mylist.insert(10, 0);
    mylist.insert(10, 0);
    std::cout << "removing: " << mylist.remove(3) << std::endl;
    std::cout << "removing: " << mylist.remove(2) << std::endl;
    std::cout << "removing: " << mylist.remove(1) << std::endl;
    std::cout << mylist.get(0) << std::endl;
    std::cout << mylist.get(1) << std::endl;
    std::cout << mylist.get(2) << std::endl;
    std::cout << mylist.get(3) << std::endl;

    system("pause");
    return 0;
}
#endif // MKLINKEDLIST_H
