#include <iostream>
#include <memory>  //shared_ptr
#include <utility>

using namespace std;


template <typename T>
class Set
{
public:
    Set();
    Set(T d);
    Set(T a[], int n);
    Set(const Set& s); //copy
    Set(Set&& s); //move
    ~Set();
    
    Set & operator=(const Set& s ); //copy asign
    Set & operator=( Set && s ); //move assign
    
    Set& operator+=(const Set& s);
    Set& operator*=(const Set& s);
    Set& operator-=(const Set& s);
    
    bool operator<=(const Set& s);
    bool operator!=(const Set& s );
    
    bool operator<(const Set& s) 
    {
        return cardinality() < s.cardinality() && operator<=(s);
    }
    bool operator==(const Set& s) 
    {
        return cardinality() == s.cardinality() && operator<=(s);
    }
    
    bool empty() const;
    int cardinality() const;
    void make_empty();
    bool is_member(const T&) const;
    
private:
    class Node
    {
    public:
        typedef shared_ptr<Node> Np;
        Node(T d = T{}, Np n = nullptr, Np p = nullptr) : data(d), next(n), prev(p) {}
        ~Node() = default;

        T data;
        shared_ptr<Node> next;
        shared_ptr<Node> prev;
    };
    
    typedef shared_ptr<Node> Np;
    Np head;
    Np tail;
    
    friend Set operator+(Set lhs, const Set& rhs) {
        return lhs+=rhs; //works nice since lhs is passed by copy!
    }
    
    friend Set operator*(Set lhs, const Set& rhs) {
        return lhs*=rhs; //works nice since lhs is passed by copy!
    }
    
    friend Set operator-(Set lhs, const Set& rhs) {
        return lhs-=rhs; //works nice since lhs is passed by copy!
    }
    
    
    friend ostream& operator<<(ostream& os, const Set& s) {
        if(s.empty()){
            os << "Set is empty!";
            return os;
        }
        os << "{ ";
        for(Np node = s.head->next; node != s.tail; node = node->next )
            os << node->data << " ";
        os << "}";
        return os;
    }
};

template <typename T>
Set<T>::Set()
{
    head = make_shared<Node>();
    tail = make_shared<Node>();
    
    head->next = tail;
    tail->prev = head;
} 

template <typename T>
Set<T>::Set(T d) : Set()
{
    Np newNode = make_shared<Node>(d);
    
    head ->next = newNode;
    tail->prev = newNode;
    
    newNode->prev = head;
    newNode->next = tail;  
}

template <typename T>
Set<T>::Set(T a[], int n) : Set()
{
    Np tmp;
    for (size_t i = 0; i < n; i++)
    {
        tmp = tail->prev;
        tmp -> next = make_shared<Node>(a[i],tail, tmp);
        tail->prev = tmp->next;
        tmp = tmp->next;
    }
}

template <typename T> 
Set<T>::Set(const Set& s) : Set()
{
    // cout << "init set with another set ( copy ) ... \n";
    Np tmp;
    if(s.empty()){
        // cout << "set to be copied is empty ... \n";
    } else {
        // cout << "creating copy of Set: " << s << endl;
        for(Np node = s.head->next; node != s.tail; node = node->next ){
            tmp = tail->prev;
            tail->prev = make_shared<Node>(node->data, tail, tmp);
            tmp ->next = tail->prev;
        }   
    }
}

template <typename T>
Set<T>::Set( Set&& s) : Set()
{
    // cout << "init set with another set ( move ) ... \n";
    head = s.head; //nodes are "stolen" form s
    tail = s.tail;
    s.head = s.tail = nullptr; //dosnt leave an empty set
}

template <typename T>
Set<T>::~Set()
{
    // cout << "destructor for SET (MOVE dosnt leave any nodes(head/tail)\n";
    Np next = head;
    Np tmp;
    
    while(next != tail){
        tmp = next;
        next = next->next;
        tmp->next = nullptr; 
    }
    head = tail = nullptr;
    //By not pointing to the nodes (shared_ptr) they should invoke 'delete' naturally
}


template <typename T>
bool Set<T>::empty() const
{
    if(head == nullptr && tail == nullptr)
        return true;
    if(head->next == tail && tail->prev == head)
        return true;
    
    return false;
}

template <typename T>
int Set<T>::cardinality() const
{
    if(empty())
        return 0;
    int count = 0;
    for (Np node = head; node->next != tail; node = node->next, count++) { }
    return count;
}

template <typename T>
void Set<T>::make_empty()
{
    if(empty())
        return;

    Np next = head->next;
    Np tmp;
    
    while(next->next != tail){
        tmp = next;
        next = next->next;
        tmp->next = nullptr; 
    }
    
    head->next = tail;
    tail->prev = head;
}

template <typename T>
bool Set<T>::is_member(const T& v) const
{
    for (Np node = head->next; node != tail; node = node->next) 
        if(node->data == v) 
            return true;
    return false;
}

template <typename T> 
Set<T>& Set<T>::operator=( const Set<T> & s ) //copy assignment
{ 
    
    //s is passed by copy = tmp variable = copy and swap 
    
    // swap(head,s.head);
    // swap(tail,s.tail);
    
    // return *this;
    // cout << "copy assign\n";
    if(!empty())
        make_empty();
    
    if(s.empty())
        return *this;
    
    Np tmp;
    for(Np node = s.head->next; node != s.tail; node = node->next){
        tmp = tail->prev;
        tail->prev = make_shared<Node>(node->data, tail, tmp);
        tmp ->next = tail->prev;
    }
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator=( Set && s )
{
    // cout << "move assign\n";
    if(!empty())
        make_empty();
    
    head = s.head; //nodes are "stolen" form s
    tail = s.tail;
    s.head = s.tail = nullptr; //dosnt leave an empty set
    
    return *this;
   
}


template <typename T>
Set<T>& Set<T>::operator+=(const Set& s )
{
    // cout << "Union operator\n";
    Np tmp;
    Np c1 = head->next;//c1 keeps track of the original set 
    for(Np c2 = s.head->next; c2 != s.tail; c2 = c2->next) //c2 itterates 's'
    {
        if(!is_member(c2->data))
        {
            //if larger than last value. insert at back
            if(c2->data > tail->prev->data){
                c1 = tail;
            } else {
                
            //else find position within set
            while(c1->data < c2->data )
                c1 = c1->next;
            }
            
            //insert at updated c1 position
            tmp = c1->prev;
            c1->prev = make_shared<Node>(c2->data,c1, tmp);
            tmp->next = c1->prev;
        }    
    }
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator-=(const Set& s )
{
    // cout << "differance operator\n";
    Np tmp;
    for(Np node = head->next; node != tail; node = node->next){
        
        if(s.is_member(node->data)) { //remove
            
            tmp = node;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            tmp = nullptr;
        }
    }
    
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator*=(const Set& s )
{
    // cout << "differance operator\n";
    Np tmp;
    for(Np node = head->next; node != tail; node = node->next){
        
        if(!s.is_member(node->data)) { //remove
            
            tmp = node;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            tmp = nullptr;
        }
    }
    
    return *this;
}

template <typename T>
bool Set<T>::operator<=(const Set& s )
{
    // cout << "subset operator\n";
    for(Np node = head->next; node != tail; node = node->next)
        if(!s.is_member(node->data))
            return false;
        return true;    
}

template <typename T>
bool Set<T>::operator!=(const Set& s )
{
    // cout << "different operator\n";
    if(cardinality() != s.cardinality())
        return true;
    for(Np node = head->next; node != tail; node = node->next)
        if(!s.is_member(node->data))
            return true;
    return false;
}