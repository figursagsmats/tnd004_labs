
/*
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: template class HashTable represents an open addressing hash table
              (also known as closed_hashing) with linear probing
*/

#include "Item.h"

#include <iostream>
#include <iomanip>

using namespace std;

const int NOT_FOUND = -1;
const double MAX_LOAD_FACTOR = 0.5;

struct idxPair {
    int matchOrEmptyIdx;
    int firstDeletedIdx;
};

//Template class to represent an open addressing hash table using linear probing to resolve collisions
//Internally the table is represented as an array of pointers to Items
template <typename Key_Type, typename Value_Type>
class HashTable
{
public:

    //New type HASH: pointer to a hash function
    typedef unsigned (*HASH)(Key_Type, int);


    //Constructor to create a hash table
    //table_size is number of slots in the table (next prime number is used)
    //f is the hash function
    HashTable(int table_size, HASH f);


    //Destructor
    ~HashTable();


    //Return the load factor of the table, i.e. percentage of slots in use or deleted
    double loadFactor() const
    {
        return (double) (nItems+nDeleted) / _size;
    }


    //Return number of items stored in the table
    unsigned get_number_OF_items() const
    {
        return nItems;
    }

    //Return the total number of visited slots (during search, insert, remove, or re-hash)
    unsigned get_total_visited_slots() const
    {
        return total_visited_slots;
    }

    //Return the total number of call to new Item()
    unsigned get_count_new_items() const
    {
        return count_new_items;
    }

    
    //Return a pointer to the value associated with key
    //If key does not exist in the table then nullptr is returned
    const Value_Type* _find(const Key_Type& key);


    //Insert the Item (key, v) in the table
    //If key already exists in the table then change the value associated with key to v
    //Re-hash if the table reaches the MAX_LOAD_FACTOR
    void _insert(const Key_Type& key, const Value_Type& v);


    //Remove Item with key, if the item exists
    //If an Item was removed then return true
    //otherwise, return false
    bool _remove(const Key_Type& key);

    //Overloaded subscript operator
    //If key is not in the table then insert a new Item = (key, Value_Type())
    Value_Type& operator[](const Key_Type& key);


    //Display all items in table T to stream os
    friend ostream& operator<<(ostream& os, const HashTable& T)
    {
        for (unsigned i = 0; i < T._size; ++i)
        {
            if (T.hTable[i] && T.hTable[i] != Deleted_Item<Key_Type,Value_Type>::get_Item())
            {
                os << *T.hTable[i] << endl;
            }
        }

        return os;
    }


    //Display the table for debug and testing purposes
    //Thus, empty and deleted entries are also displayed
    void display(ostream& os);


private:

    /* ********************************** *
    * Data members                        *
    * *********************************** */

    //Number of slots in the table, a prime number
    unsigned _size;

    //Hash function
    const HASH h;

    //Number of items stored in the table
    //Instances of Deleted_Items are not counted
    unsigned nItems;

    //Number of slots that are marked as deleted
    unsigned nDeleted;

    //Table is an array of pointers to Items
    //Each slot of the table stores a pointer to an Item =(key, value)
    Item<Key_Type, Value_Type>** hTable;

    //Some statistics
    unsigned total_visited_slots;  //total number of visited slots
    unsigned count_new_items;      //number of calls to new Item()


    /* ********************************** *
    * Auxiliar member functions           *
    * *********************************** */
    void rehash();
    idxPair locateIdxs(const Key_Type& key);

    //Disable copy constructor!!
    HashTable(const HashTable &) = delete;

    //Disable assignment operator!!
    const HashTable& operator=(const HashTable &) = delete;

};


//Test if a number is prime
bool isPrime( int n );

//Return a prime number at least as large as n
int nextPrime( int n );


/* ********************************** *
* Member functions implementation     *
* *********************************** */

//Constructor to create a hash table
//table_size number of slots in the table (next prime number is used)
//f is the hash function
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::HashTable(int table_size, HASH f)
    : h(f)
{
    //IMPLEMENT
    _size = nextPrime(table_size); 
    nDeleted = nItems = total_visited_slots = count_new_items = 0; 
    
    hTable = new Item<Key_Type, Value_Type>*[_size]{nullptr}; //allocate memory for the table, init with nullptr
}


//Destructor
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::~HashTable()
{
    //IMPLEMENT
    for(int i = 0; i < _size; i++)
    {
        //cout << i << endl;
        if(hTable[i] == nullptr) 
        {    
            continue;
        }
        else if(hTable[i] == Deleted_Item<Key_Type, Value_Type>::get_Item())
        {
            continue; 
        }
        else 
        {
            delete hTable[i]; //free only the allocate memory
        }
    }
    delete[] hTable; //delete the dynamiclly allocated pointers asswell
}


//Return a pointer to the value associated with key
//If key does not exist in the table then nullptr is returned
template <typename Key_Type, typename Value_Type>
const Value_Type* HashTable<Key_Type, Value_Type>::_find(const Key_Type& key)
{
    idxPair idxs = locateIdxs(key);
    
    bool isMatch = !!hTable[idxs.matchOrEmptyIdx];
    
    if(isMatch) 
    {
        if(idxs.firstDeletedIdx == NOT_FOUND)
        {
            return &hTable[idxs.matchOrEmptyIdx]->get_value();
        } 
        else 
        {
            hTable[idxs.firstDeletedIdx] = hTable[idxs.matchOrEmptyIdx];
            hTable[idxs.matchOrEmptyIdx] = nullptr;
            return &hTable[idxs.firstDeletedIdx]->get_value();     
        }
    }
    else
    {
        return nullptr;
    }
}


//Insert the Item (key, v) in the table
//If key already exists in the table then change the value associated with key to v
//Re-hash if the table reaches the MAX_LOAD_FACTOR
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::_insert(const Key_Type& key, const Value_Type& v)
{
    idxPair idxs = locateIdxs(key);
    
    bool slotIsEmpty = !hTable[idxs.matchOrEmptyIdx];
    
    if(slotIsEmpty) 
    {
        if(idxs.firstDeletedIdx == NOT_FOUND)
        {
            hTable[idxs.matchOrEmptyIdx] = new Item<Key_Type, Value_Type>{key, v};
        } 
        else 
        {
            hTable[idxs.firstDeletedIdx] = new Item<Key_Type, Value_Type>{key, v};
        }
        count_new_items++;
        nItems++;
    }
    else
    {
        hTable[idxs.matchOrEmptyIdx]->set_value(v);   
    }
    
    if(loadFactor() > MAX_LOAD_FACTOR)
    {
         rehash();
    } 
}


//Remove Item with key, if the item exists
//If an Item was removed then return true
//otherwise, return false
template <typename Key_Type, typename Value_Type>
bool HashTable<Key_Type, Value_Type>::_remove(const Key_Type& key)
{
    idxPair idxs = locateIdxs(key);
    
    bool slotIsEmpty = !hTable[idxs.matchOrEmptyIdx];
    
    if(slotIsEmpty) 
    {
        return false;
    }
    else
    {
        delete hTable[idxs.matchOrEmptyIdx];   
        nItems --;
        nDeleted ++;
        hTable[idxs.matchOrEmptyIdx] = Deleted_Item<Key_Type, Value_Type>::get_Item();
        return true;
    }
}
template <typename Key_Type, typename Value_Type>
Value_Type& HashTable<Key_Type, Value_Type>::operator[](const Key_Type& key)
{
    if(loadFactor() > MAX_LOAD_FACTOR)
    {
        rehash();
    }
    
    idxPair idxs = locateIdxs(key);
    
    bool isMatch = !!hTable[idxs.matchOrEmptyIdx];
    
    if(isMatch) 
    { 
        if(idxs.firstDeletedIdx == NOT_FOUND)
        {
            return hTable[idxs.matchOrEmptyIdx]->get_value();
        } 
        else 
        {
            hTable[idxs.firstDeletedIdx] = hTable[idxs.matchOrEmptyIdx];
            hTable[idxs.matchOrEmptyIdx] = nullptr;
            return hTable[idxs.firstDeletedIdx]->get_value();     
        }
    }
    else
    {
        count_new_items++;
        nItems++;
        if(idxs.firstDeletedIdx == NOT_FOUND)
        {
            hTable[idxs.matchOrEmptyIdx] = new Item<Key_Type, Value_Type>{key, Value_Type{}};
            return hTable[idxs.matchOrEmptyIdx]->get_value();
        }
        else
        {
            hTable[idxs.firstDeletedIdx] = new Item<Key_Type, Value_Type>{key, Value_Type{}};
            return hTable[idxs.firstDeletedIdx]->get_value(); 
        }
    }
    cout << "WARNING!! ended up at bottom in []operator" << endl;
    return hTable[idxs.matchOrEmptyIdx]->get_value();  
}

//Display the table for debug and testing purposes
//This function is used for debugging and testing purposes
//Thus, empty and deleted entries are also displayed
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::display(ostream& os)
{
    os << "-------------------------------\n";
    os << "Number of items in the table: " << get_number_OF_items() << endl;
    os << "TAble size: " << _size << endl;
    os << "Load factor: " << fixed << setprecision(2) << loadFactor() << endl;

    for (unsigned i = 0; i < _size; ++i)
    {
        os << setw(6) << i << ": ";

        if (!hTable[i])
        {
            os << "null" << endl;
        }
        else if (hTable[i] == Deleted_Item<Key_Type, Value_Type>::get_Item())
        {
            os << "deleted" << endl;
        }
        else
        {
            os << *hTable[i]
               << "  (" << h(hTable[i]->get_key(),_size) << ")" << endl;
        }
    }

    os << endl;
}


/* ********************************** *
* Auxiliar member functions           *
* *********************************** */
//Add any if needed
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::rehash()
{
    cout << "rehashing!\n";
    //uppdate the members to fit the new table
    int oldsize = _size;
    nDeleted = nItems = count_new_items = 0; //borde statistik variablerna nollas?
    Item<Key_Type, Value_Type>** oldTable = hTable; //a new pointer to the old table

    _size = nextPrime(_size * 2); //allocate new table at 2x size
    hTable = new Item<Key_Type, Value_Type>*[_size] {nullptr}; //no safety here.. assumes that there allways will be a new allocation available

    for (int i = 0; i < oldsize; ++i)
    {
        if(oldTable[i] != nullptr && oldTable[i] != Deleted_Item<Key_Type, Value_Type>::get_Item())
        {
            //insert values from old nodes and dealocate the objects pointed to
            _insert(oldTable[i]->get_key(), oldTable[i]->get_value() ); //insert from old to the new table
            delete oldTable[i];
        }
    }
    //dealocate the pointers
    delete[] oldTable;

    //should not end up here
    if(loadFactor() > MAX_LOAD_FACTOR){
        cout << "OBS! Recursive rehash call!\n";
        rehash();
    }
}

template <typename Key_Type, typename Value_Type>
idxPair HashTable<Key_Type, Value_Type>::locateIdxs(const Key_Type& key)
{
       int idx = h(key, _size);
       int startIdx = idx;
       int firstDeletedIdx = NOT_FOUND;
       
       while(1)
       {
           total_visited_slots++;
           
           if(!hTable[idx] || hTable[idx]->get_key() == key)
           {
               break;
           }
           
           if(hTable[idx] == Deleted_Item<Key_Type,Value_Type>::get_Item() )
           {
               firstDeletedIdx = (firstDeletedIdx == NOT_FOUND) ? idx : firstDeletedIdx;
           }
           
           idx++;
           
           if(idx == startIdx)
           {
               idx = NOT_FOUND;
               break;
           }
           
           if(idx == _size)
           {
               idx = 0;
           }
       }
       return {idx, firstDeletedIdx};
}

/* ********************************** *
* Functions to find prime numbers     *
* *********************************** */


//Test if a number is prime
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}


//Return a prime number at least as large as n
int nextPrime( int n )
{
    if( n % 2 == 0 )
        n++;

    for(; !isPrime( n ); n += 2 );

    return n;
}
