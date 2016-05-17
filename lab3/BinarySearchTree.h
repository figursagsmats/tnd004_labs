#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "dsexceptions.h"
#include <algorithm>
#include <memory>
#include <iomanip>
using namespace std;

// #define EX3

// BinarySearchTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted
template <typename Comparable>
struct predSuccPair {
  Comparable a;
  Comparable b;
};

template <typename Comparable>
class BinarySearchTree
{
  public:
    BinarySearchTree( ) : root{ nullptr }
    {
    }

    /**
     * Copy constructor
     */
    BinarySearchTree( const BinarySearchTree & rhs ) : root{ nullptr }
    {
        root = clone( rhs.root );
        link(root);
    }

    /**
     * Move constructor
     */
    BinarySearchTree( BinarySearchTree && rhs ) : root{ rhs.root }
    {
        rhs.root = nullptr;
    }

    /**
     * Destructor for the tree
     */
    ~BinarySearchTree( )
    {
        makeEmpty( );
    }

    /**
     * Copy assignment
     */
    BinarySearchTree & operator=( const BinarySearchTree & rhs )
    {
        BinarySearchTree copy = rhs;
        std::swap( *this, copy );
        return *this;
    }

    /**
     * Move assignment
     */
    BinarySearchTree & operator=( BinarySearchTree && rhs )
    {
        std::swap( root, rhs.root );
        return *this;
    }

    /**
     * Find the smallest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMin( root )->element;
    }

    /**
     * Find the largest item in the tree.
     * Throw UnderflowException if empty.
     */
    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return findMax( root )->element;
    }

    /**
     * Returns true if x is found in the tree.
     */


     /**This contains invokes the BOOL version - The BiIterator version is further down */
    #ifndef EX3
    bool contains( const Comparable & x )
    {
        return contains1( x, root );
    }
    #endif

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty( ) const
    {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree( ostream & out = cout ) const
    {
        if( isEmpty( ) )
            out << "Empty tree" << endl;
        else
            printTree( root, out );
    }

    /**
     * Make the tree logically empty.
     */
    void makeEmpty( )
    {
        makeEmpty( root );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( const Comparable & x )
    {
        insert( x, root, weakPtr{} );
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert( Comparable && x )
    {
        insert( std::move( x ), root, weakPtr{} );
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove( const Comparable & x )
    {
        remove( x, root );
    }

    Comparable get_parent(const Comparable &c) const
    {
        strongPtr t = root;
        while(t != nullptr)
        {
            if(t->element == c)
            {
                break;
            }
            else if(t->element > c)
            {
                t = t->left;
            }
            else
            {
                 t = t->right;
            }
        }
        if(t == nullptr)
            return Comparable{};
        else
            if(t->parent.lock() != nullptr)
                return t->parent.lock()->element;
            else
                return Comparable{};
    }

    bool find_pred_succ(const Comparable& c, Comparable& a, Comparable& b) const
    {
        find_pred_succ_not_in_tree(c,a,b,root);
        
        return true;
        
        strongPtr t = root;
        while(t != nullptr)
        {
            if(t->element == c)
            {
                break;
            }
            else if(t->element > c)
            {
                t = t->left;
            }
            else
            {
                 t = t->right;
            }
        }
        if(t == nullptr)
        {
            strongPtr tmp = root;

            find_pred_succ_not_in_tree(c, a, b, root);

            return false;
        }

        strongPtr tmp;

        //find pred
        if(t->left != nullptr)
            a = findMax(t->left)->element;
        else
        {
            tmp = t;
            a = t->element;
            while(tmp != nullptr && tmp != root)
            {
                if((tmp->parent.lock() && tmp->parent.lock()->right == tmp )|| (tmp->parent.lock() == root && root->element < t->element))
                   {
                      a = tmp->parent.lock()->element;
                      break;
                   }

                tmp = tmp->parent.lock();
            }
        }

        //find succesor
        if(t->right != nullptr)
            b = findMin(t->right)->element;
        else
        {
            tmp = t;
            b = t->element;
            while(tmp != nullptr && tmp != root)
            {
                if((tmp->parent.lock() && tmp->parent.lock()->left == tmp )|| (tmp->parent.lock() == root && root->element > t->element))
                {
                    b = tmp->parent.lock()->element;
                    break;
                }
                tmp = tmp->parent.lock();
            }
        }
        return true;
    }

  private:
    struct BinaryNode;

    typedef shared_ptr<BinaryNode> strongPtr;
    typedef weak_ptr<BinaryNode> weakPtr;

    struct BinaryNode
    {
        Comparable element;

        strongPtr left;
        strongPtr right;

        weakPtr parent;

        BinaryNode( const Comparable & theElement, strongPtr lt, strongPtr rt, weakPtr p )
          : element{ theElement }, left{ lt }, right{ rt }, parent{ p } { }

        BinaryNode( Comparable && theElement, strongPtr lt, strongPtr rt, weakPtr p )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt }, parent{ p } { }
    };

    strongPtr root;

public:
    class BiIterator
    {
    private:
        BinarySearchTree* bst;
        strongPtr current;
    public:
        BiIterator() {
            current = nullptr;
            bst = nullptr;
        };
        BiIterator(strongPtr t, BinarySearchTree* b) : current{t}, bst{b} {};

        Comparable& operator*() const
        {
            return this->current->element;
        }
        strongPtr operator->() const
        {
            return current;
        }
        bool operator==(const BiIterator &it) const
        {
            return this->current->element == it.current->element;
        }
        bool operator!=(const BiIterator &it) const
        {
            return this->current->element != it.current->element;
//            return !(*this == it);
        }
        //pre
        BiIterator& operator++()
        {
            strongPtr t = current, tmp;
            if(t->right != nullptr)
            {
                t = bst->findMin(t->right);
            }
            else
            {
                tmp = t;
                while(tmp != nullptr && tmp != bst->root)
                {
                    if((tmp->parent.lock() && tmp->parent.lock()->left == tmp )|| (tmp->parent.lock() == bst->root && bst->root->element > t->element))
                    {
                        t = tmp->parent.lock();
                        break;
                    }
                    tmp = tmp->parent.lock();
                }
            }
            if(current == t)
            {
                current =  make_shared<BinaryNode>(Comparable{},strongPtr{},strongPtr{},weakPtr{});
            }
            else
            {
                current = t;
            }
            return *this;
        }
        //post
        BiIterator operator++(int)
        {
            strongPtr t = current, tmp;
            BiIterator tmpIterator = *this;
            if(t->right != nullptr)
            {
                t = bst->findMin(t->right);
            }
            else
            {
                tmp = t;
                while(tmp != nullptr && tmp != bst->root)
                {
                    if((tmp->parent.lock() && tmp->parent.lock()->left == tmp )|| (tmp->parent.lock() == bst->root && bst->root->element > t->element))
                    {
                        t = tmp->parent.lock();
                        break;
                    }
                    tmp = tmp->parent.lock();
                }
            }
            
            if(current == t)
            {
                current =  make_shared<BinaryNode>(Comparable{},strongPtr{},strongPtr{},weakPtr{});
            }
            else
            {
                current = t;
            }
            return tmpIterator;
        }
        //pre
        BiIterator& operator--()
        {
            strongPtr t = current, tmp;
            if(t->left != nullptr)
            {
                t = bst->findMax(t->left);
            }
            else
            {
                tmp = t;
                while(tmp != nullptr && tmp != bst->root)
                {
                    if((tmp->parent.lock() && tmp->parent.lock()->right == tmp )|| (tmp->parent.lock() == bst->root && bst->root->element < t->element))
                    {
                        t = tmp->parent.lock();
                        break;
                    }
                    tmp = tmp->parent.lock();
                }
            }
            if(current == t)
            {
                current =  make_shared<BinaryNode>(Comparable{},strongPtr{},strongPtr{},weakPtr{});
            }
            else
            {
                current = t;
            }
            return *this;
        }
        //post
        BiIterator operator--(int)
        {
            strongPtr t = current, tmp;
            BiIterator tmpIterator = *this;
            if(t->left != nullptr)
            {
                t = bst->findMax(t->left);
            }
            else
            {
                tmp = t;
                while(tmp != nullptr && tmp != bst->root)
                {
                    if((tmp->parent.lock() && tmp->parent.lock()->right == tmp )|| (tmp->parent.lock() == bst->root && bst->root->element < t->element))
                    {
                        t = tmp->parent.lock();
                        break;
                    }
                    tmp = tmp->parent.lock();
                }
            }
            if(current == t)
            {
                current =  make_shared<BinaryNode>(Comparable{},strongPtr{},strongPtr{},weakPtr{});
            }
            else
            {
                current = t;
            }
            return tmpIterator;
        }
    };

    BiIterator begin()
    {
        return BiIterator(findMin(root), this);
    }
    BiIterator end()
    {
        return BiIterator{ make_shared<BinaryNode>(Comparable{},strongPtr{},strongPtr{}, weakPtr{} ), this};
    }
    
    #ifdef EX3
   BiIterator contains( const Comparable & x )
   {
       return contains2( x, root );
   }
   
   #endif
private:

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, strongPtr & t, weakPtr p )
    {
        if( t == nullptr )
            t = make_shared<BinaryNode>(x, nullptr, nullptr, p);
        else if( x < t->element )
            insert( x, t->left, t );
        else if( t->element < x )
            insert( x, t->right, t );
        else
        {
            #ifndef EX3
            ++(t->element);  // Duplicate; do nothing  !!!!! FOR EX 4 ONLY
            #endif
        }
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable && x, strongPtr & t, weakPtr p )
    {
        if( t == nullptr )
            t = make_shared<BinaryNode>(x, nullptr, nullptr, p);
        else if( x < t->element )
            insert( std::move( x ), t->left, t );
        else if( t->element < x )
            insert( std::move( x ), t->right, t );
        else
        {
            #ifndef EX3
            ++(t->element);  // Duplicate; do nothing, !!! FOR EX 4 ONLY
            #endif
        }
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, strongPtr & t )
    {
        if( t == nullptr )
            return;   // Item not found; do nothing
        if( x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left != nullptr && t->right != nullptr ) // Two children
        {
//            Comparable tmp = t->element;
            t->element = findMin( t->right )->element;
            remove( t->element, t->right );
        }
        else if (t->left == nullptr && t->right == nullptr)
        {
            t = nullptr;
//            printTree();
        }
        else //1 child
        {
            strongPtr oldNode = t;
            t = ( t->left != nullptr ) ? t->left : t->right;

            if(oldNode->parent.lock()) //root = no parent
                t->parent = oldNode->parent.lock();

//            printTree();
            oldNode = nullptr;
        }
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    strongPtr findMin( strongPtr t ) const
    {
        if( t == nullptr )
            return nullptr;
        if( t->left == nullptr )
            return t;
        return findMin( t->left );
    }

    /**
     * Internal method to find the largest item in a subtree t.
     * Return node containing the largest item.
     */
    strongPtr findMax( strongPtr t ) const
    {
        if( t != nullptr )
            while( t->right != nullptr )
                t = t->right;
        return t;
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the subtree.
     */
    bool contains1( const Comparable & x, strongPtr t ) const
    {
        if( t == nullptr )
            return false;
        else if( x < t->element )
            return contains1( x, t->left );
        else if( t->element < x )
            return contains1( x, t->right );
        else
            return true;    // Match
    }
/****** NONRECURSIVE VERSION*************************/
    BiIterator contains2( const Comparable & x, strongPtr t )
    {
        BiIterator it;
        for(it = begin(); it != end(); it++)
        {
            if(it->element == x)
                return it;
        }
        return end();   // No match
    }
    
    private:


    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty( strongPtr & t )
    {
//        if( t != nullptr )
//        {
//            makeEmpty( t->left );
//            makeEmpty( t->right );
//            delete t;
//        }
        t = nullptr;  //this should be all we need to do, if the parent pointers are weak!
    }

    //recursive in-order display
    void display( strongPtr t, ostream & out, int w = {2} ) const
    {
        if( t != nullptr )
        {
            out << setw(w) << t->element << ",p:";
            if(auto tmp = t->parent.lock())
                cout << tmp->element << endl;
            else
                cout << endl;
            display( t->left, out, w+4);
            display( t->right, out, w+4 );
        }
    }

    /**
     * Internal method to print a subtree rooted at t in sorted order.
     * In-order traversal is used
     */
    void printTree( strongPtr t, ostream & out ) const
    {
        if( t != nullptr )
        {
            display(t, out);
//            out << t->element << endl;
//            printTree( t->left, out );
//            printTree( t->right, out );
        }
    }

    /**
     * Internal method to clone subtree.
     */
    strongPtr clone( strongPtr t ) const
    {
        strongPtr tmp;
        if( t == nullptr )
        {
            return nullptr;
        }

        else
        {
            tmp = make_shared<BinaryNode>( t->element, clone( t->left ), clone( t->right ), weakPtr{} );
        }

        if(tmp->right)
            tmp->right->parent = tmp;
        if(tmp->left)
            tmp->left->parent = tmp;
        
        return tmp;

    //returnerar weakPtr{} ist�ller f�r pekare till inte �nnu skapat objekt...
    //l�nkar ihop parents med void link()... löst?
    }
    void link(strongPtr t)
    {
//        if(t->left)
//        {
//           t->left->parent = t;
//           link(t->left);
//        }
//        if(t->right)
//        {
//           t->right->parent = t;
//           link(t->right);
//        }
    }

    void find_pred_succ_not_in_tree(const Comparable& c, Comparable& a, Comparable& b, strongPtr t ) const
    {
        if(c > t->element)
        {
            a = t->element;
            if(t->right)
                find_pred_succ_not_in_tree(c, a, b, t->right);
        }
        else if(c < t->element)
        {
            b = t->element;
            if(t->left)
                find_pred_succ_not_in_tree(c, a, b, t->left);
        }
        else //found c
        {
            if(t->left)
                find_pred_succ_not_in_tree(c, a, b, t->left);
            if(t->right)
                find_pred_succ_not_in_tree(c, a, b, t->right);   
        }
    }
};

#endif
