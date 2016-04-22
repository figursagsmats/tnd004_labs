#include <memory>
#include <iostream>
//#include "node.h"

using namespace std;

template <typename T>
class Set
{
public:
    //CONSTRUCTORS ETC
    Set(); //default constructor
    Set(const T& data); //EDIT: of course
    Set(T data[],int size);
    Set(const Set& s);  //copy constructor
    Set(Set&& s); //move constructor

    ~Set() = default; //desctructor

    //OPERATORS
    Set& operator=(const Set& s); //copy assign
    Set& operator=(Set&& s); //move assign
    Set& operator+=(const Set& s); //union
    Set& operator*=(const Set& s); //intersection
    Set& operator-=(const Set& s); //difference

    bool operator<=(const Set& s); //subset
    bool operator!=(const Set& s); //different

    bool operator<(const Set& s)
    {
        return cardinality() > s.cardinality() && operator<=(s);
    }
    bool operator==(const Set& s)
    {
        return cardinality() == s.cardinality() && operator<=(s);
    }
    //METHODS
    bool isEmpty() const;
    void make_empty();
    bool is_member(const T&) const;
    int cardinality() const;


private:
    class Node {
    public:
        typedef shared_ptr<Node> NodePtr;
        typedef weak_ptr<Node> wNodePtr;
        NodePtr next;
        wNodePtr prev;
        T data;


        Node(T d = T{}, const NodePtr& np = nullptr , const wNodePtr& pp = weak_ptr<Node>())
        {
            data = d;
            next = np;
            prev = pp;

        }
    };

    typedef shared_ptr<Node> NodePtr;

    //PRIVATE VARIABLES
    NodePtr head;
    NodePtr tail;
    int nNodes = 0;

    //PRIVATE METHODS

    friend Set operator+(Set lhs, const Set& rhs) {
        return lhs+=rhs; //works nice since lhs is passed by copy!
    }

    friend Set operator*(Set lhs, const Set& rhs) {
        return lhs*=rhs; //works nice since lhs is passed by copy!
    }

    friend Set operator-(Set lhs, const Set& rhs) {
        return lhs-=rhs; //works nice since lhs is passed by copy!
    }

    friend ostream& operator<<(ostream& os, const Set(& s)){
        cout << "{ ";
        if(s.isEmpty()){
           os << "EMPTY ";
        } else{
            for(NodePtr np = s.head->next; np != s.tail; np=np->next){
                os << np->data << " ";
            }
        }
        cout << "}" ;
        return os;
    }

    void insertLast(T data) {
        nNodes++;
        tail->prev.lock()->next = make_shared<Node>(data,tail,tail->prev);
        tail->prev = tail->prev.lock()->next;
    }
    void insertLeftOf(NodePtr pos, T data) {
        nNodes++;
        
        NodePtr before = pos->prev.lock();
        NodePtr node = make_shared<Node>(data,pos,pos->prev.lock());
        pos->prev = node;
        before->next = node;
    }
    void deleteAt(NodePtr node) {
        nNodes--;
        NodePtr tmp = node;
        node->prev.lock()->next = node->next;
        node->next->prev = node->prev;
        tmp = nullptr;
    }
};

//CONSTRUCTORS ETC

// default constructor
template <typename T>
Set<T>::Set(){
    //cout << "Set default constructor" << endl;

    //Create an empty set. This means head points to tail an vice versa
    head = make_shared<Node>();
    tail = make_shared<Node>();

    head->next = tail;
    //head->prev.lock(); = nullptr;

    //tail->next = nullptr;
    tail->prev = head;

}

//assignment
template <typename T>
Set<T>::Set(const T& data) : Set(){

    head->next = make_shared<Node>(data,tail,head);
    tail->prev = head->next;

}

//assignment
template <typename T>
Set<T>::Set(T data[],int size) : Set() {
    //loop array assuming that user input correct size (retarded)
    for(int i = 0; i<size;i++){
        insertLast(data[i]);
    }
}

//copy constructor
template <typename T>
Set<T>::Set(const Set& s) : Set()
{
    //Task 2.1

    if(s.isEmpty()){
    } else {
        for(NodePtr node = s.head->next; node != s.tail; node = node->next ){
            insertLast(node->data);
        }
    }
}

//move constructor
template <typename T>
Set<T>::Set( Set&& s) : Set()
{
    cout << "Move constructor called..." << endl;
    head = s.head; //nodes are "stolen" form s
    tail = s.tail;
    s.head = s.tail = nullptr; //dosnt leave an empty set
}

// //desctructor
// template <typename T>
// Set<T>::~Set(){
//     // cout << "destructor for SET (MOVE dosnt leave any nodes(head/tail)\n";
//     NodePtr next = head;
//     NodePtr tmp;

//     while(next != tail){
//         tmp = next;
//         next = next->next;
//         tmp->next = nullptr;
//     }
//     head = tail = nullptr;
//     //By not pointing to the nodes (shared_ptr) they should invoke 'delete' naturally
// }

//OPERATORS

//assignment operator
template <typename T>
Set<T>& Set<T>::operator=( const Set<T> & s ){
    //Task 2.2

    if(!isEmpty()) {
        make_empty();
    }

    if(s.isEmpty()) {
        return *this;
    }

    for(NodePtr node = s.head->next; node != s.tail; node = node->next){
        insertLast(node->data);
    }
    return *this;


}

//move operator
template <typename T>
Set<T>& Set<T>::operator=( Set && s ){
    if(!isEmpty()) {
        make_empty();
    }

    head = s.head; //nodes are "stolen" form s
    tail = s.tail;
    s.head = s.tail = nullptr; //dosnt leave an empty set

    return *this;

}

template <typename T>
Set<T>& Set<T>::operator+=(const Set& s )
{
    
    NodePtr srcPtr = s.head->next; //source pointer
    NodePtr trgPtr = head->next; //target pointer

    while(srcPtr != s.tail){
        
        if(trgPtr == tail || srcPtr->data < trgPtr->data){
            insertLeftOf(trgPtr,srcPtr->data);
            srcPtr=srcPtr->next;
        }
        else if(srcPtr->data > trgPtr->data){
            trgPtr=trgPtr->next;
        }
        else {
            trgPtr=trgPtr->next;
            srcPtr=srcPtr->next;
        }
    }
    return *this;
}


template <typename T>
Set<T>& Set<T>::operator-=(const Set& s )
{
    NodePtr tmp;
    for(NodePtr node = head->next; node != tail; node = node->next){

        if(s.is_member(node->data)) { //remove

            deleteAt(node);
        }
    }
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator*=(const Set& s )
{

    // cout << "differance operator\n";
    NodePtr tmp;

    for(NodePtr node = head->next; node != tail; node = node->next){

        if(!s.is_member(node->data)) { //remove

            deleteAt(node);
        }
    }

    return *this;
}

template <typename T>
bool Set<T>::operator<=(const Set& s )
{
    // Set s is a subset of R(this) if and only if every member
    // of s is a member of R(this)
    for(NodePtr node = head->next; node != tail; node = node->next)
        if(!s.is_member(node->data)){
            return false;
        }
        return true;
}

template <typename T>
bool Set<T>::operator!=(const Set& s )
{
    //differen operator.
    if(cardinality() != s.cardinality()){
        return true;
    }
    for(NodePtr node = head->next; node != tail; node = node->next){
        if(!s.is_member(node->data)){
            return true;
        }
    }
    return false;
}

//METHODS
template <typename T>
bool Set<T>::isEmpty() const {

    //This can only happen in copy constructor or assignment operator
    if(head == nullptr && tail == nullptr) {
        return true;
    }
    if(tail->prev.lock() == head || head->next == tail){
        return true;
    }
    return false;
}


template <typename T>
void Set<T>::make_empty(){
    if(isEmpty()){
        return;
    }

    head->next = tail;
    tail->prev = head;
}

template <typename T>
bool Set<T>::is_member(const T& v) const
{
    for (NodePtr node = head->next; node != tail; node = node->next){
        if(node->data == v) {
            return true;
        }
    }
    return false;
}

template <typename T>
int Set<T>::cardinality() const
{
    return nNodes;
}
