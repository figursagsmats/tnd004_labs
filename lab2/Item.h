/*
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: template class Item and derived class Deleted_Item
*/

#include <iostream>
#include <iomanip>
#include <new>

using namespace std;


 //Template class to represent an item stored in an hash table
 //item = (key,value)
template <typename Key_Type, typename Value_Type>
class Item
{
public:

    //Constructor to create an item given a key k and a value v
    explicit Item(const Key_Type& k, const Value_Type& v)
        : key(k) , value(v) {  }


    //Return item's key
    const Key_Type& get_key() const
    {
        return key;
    }

    //Return item's value
    //Note: a reference is returned -- thus, client code can modify the item's value
    Value_Type& get_value()
    {
        return value;
    }

    //Modify the item's value to v
    void set_value(const Value_Type& v)
    {
        value = v;
    }

protected:

    //data members
    const Key_Type key;
    Value_Type value;

    friend ostream& operator<<(ostream& os, const Item& i)
    {
        os << "key = " << "\"" << i.key << "\""
           << setw(12) << "value = " << i.value;

        return os;
    }
};

/* ********************************** *
* Derived class Deleted_Item          *
* *********************************** */


 //Template class to represent a deleted item
 //One unique instance of this class is used to mark deleted slots of the hash table
template <typename Key_Type, typename Value_Type>
class Deleted_Item: public Item<Key_Type,Value_Type>
{
public:

    //Return pointer to the item used to mark deleted entries in the table
    static Deleted_Item *get_Item()
    {
        if (!entry)
        {
            entry = new Deleted_Item();

            //cout << "Dummy item created!!" << endl;
        }

        return entry;
    }

private:

    //Only one instance of the class is needed to mark deleted slots of the table
    static Deleted_Item *entry;

    //Default constructor
    //Private member function so that only member functions can create class instances
    Deleted_Item()
        : Item<Key_Type,Value_Type>(Key_Type(), Value_Type()) { }

};


//Initialize static data member (entry)
template <typename Key_Type, typename Value_Type>
Deleted_Item<Key_Type,Value_Type> *Deleted_Item<Key_Type,Value_Type>::entry = nullptr;
