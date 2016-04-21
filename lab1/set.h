#include <memory>
#include <iostream>
//#include "node.h"

using namespace std;

template <typename T>
class Set
{
public:
    Set(); //default constructor
    Set(T data);
    Set(const Set& s); //copy constructor
    Set(T data[],int size);
    //operator=(const T&); //assignment
    ~Set(); //desctructor
    
    bool isEmpty() const;
    void insertLast(T data);
    
private:
    class Node
    {
    public:
        typedef shared_ptr<Node> NodePtr;
        
        NodePtr next;
        NodePtr prev;
        T data;
        int id;
        
        Node(T d = T{}, NodePtr np = nullptr , NodePtr pp = nullptr,int i = -1)
        {
            //cout << "Node constructor" << endl;
            data = d;
            next = np;
            prev = pp;
            id = i;
        }       
    };
    typedef shared_ptr<Node> NodePtr;    
    //PRIVATE VARIABLES
    NodePtr head;  
    NodePtr tail;
    int nNodes = 0;
    
    //PRIVATE METHODS
    friend ostream& operator<<(ostream& os, const Set(& s)){
        if(s.isEmpty()){
           os << "EMPTY"; 
        } else{
            for(NodePtr np = s.head->next; np != s.tail; np=np->next){
                os << np->data << " ";   
            }
                   
        } 
        return os;
    }
};


template <typename T>
Set<T>::Set()
{
    //cout << "Set default constructor" << endl;
    
    //Create an empty set. This means head points to tail an vice versa
    head = make_shared<Node>();
    tail = make_shared<Node>();
    
    head->next = tail;
    head->prev = nullptr;
    
    tail->next = nullptr;
    tail->prev = head;
    
}




template <typename T>
Set<T>::Set(T data) : Set()
{
    
    head->next = make_shared<Node>(data,tail,head);
    tail->prev = head->next;
    
}

template <typename T>
Set<T>::Set(T data[],int size) : Set() {
    //loop array assuming that user input correct size (retarded)
    
    for(int i = 0; i<size;i++){

        insertLast(data[i]);
    }
}

template <typename T>
Set<T>::Set(const Set& s) : Set()
{
}

template <typename T>
Set<T>::~Set()
{
}


template <typename T>
bool Set<T>::isEmpty() const
{
    if(tail->prev == head || head->next == tail){
        return true;
    }
    return false;
}
template <typename T>
void Set<T>::insertLast(T data)
{
    
    //cout << "hep" << tail->prev->next << endl;
    nNodes++;
    tail->prev->next = make_shared<Node>(data,tail,tail->prev,nNodes);
    tail->prev = tail->prev->next;
}