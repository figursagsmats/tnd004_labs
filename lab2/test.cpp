/*
  Name: hashTable.h
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: exercise 1
*/


#include <iostream>
#include <string>

#include "hashTable.h"

using namespace std;


unsigned my_hash(string s, int tableSize);

int menu();


//Test the code
int main()
{
    const int TABLE_SIZE = 7;

    HashTable<string,int> table(TABLE_SIZE, my_hash);

    string key;
    const int* p_value = nullptr;
    int value = 0;

    int choice;
    bool go = true;

    while( go )

    {
        choice = menu();

        switch(choice)
        {
        case 1:

            cout << "Enter value to be inserted: ";
            cin >> value;

            cout << "Enter key for the value to be inserted: ";
            cin >> key;

            table._insert(key, value);
            break;

        case 2:

            cout << "Enter key of the value to be searched: ";
            cin >> key;

            if ( !(p_value = table._find(key)) )
            {
                cout << "No element found at key " << key << endl;
            }
            else
            {
                cout << "value at key " << key << ": ";
                cout << *p_value << endl;
            }
            break;

        case 3:
            cout << "Enter key of the value to be deleted: ";
            cin >> key;

            if (!table._remove(key))
                cout << "Deletion failed!!" << endl;
            break;

        case 4:
            table.display(cout);
            break;

        case 5:
            go = false;
            break;

        default:
            cout << "\nEnter correct option\n";
        }
    }

    return 0;
}


unsigned my_hash(string s, int tableSize)
{
    unsigned hashVal = 0;

    for(unsigned i = 0; i < s.length(); i++)
        hashVal += s[i];

    hashVal %= tableSize;

    return hashVal;
}


//Test the code
int menu()
{
    int choice = 0;

    cout << "\n-------------------------" << endl;
    cout << "Operations on Hash Table" << endl;
    cout << "\n-------------------------" << endl;

    cout << "1. Insert " << endl;
    cout << "2. Search" << endl;
    cout << "3. Delete" << endl;
    cout << "4. Dump table" << endl;
    cout << "5. Exit" << endl;

    cout << "Enter your choice: ";

    cin >> choice;

    return choice;
}


