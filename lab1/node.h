    #include <iostream>
    #include <memory>
    
    using namespace std;
    
    template <typename T>
    class Node
    {
    public:
        typedef shared_ptr<Node> NodePtr;
        
        NodePtr nextPtr;
        NodePtr prevPtr;
        T data;
        
        Node(T d = T{}, NodePtr np = nullptr , NodePtr pp = nullptr)
        {
            cout << "Node constructor" << endl;
            data = d;
            nextPtr = np;
            prevPtr = pp;
        }       
    };
    
    