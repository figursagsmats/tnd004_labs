/*
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: exercise 2
*/


#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <random>

#include "hashTable.h"

using namespace std;

const unsigned SEED = 1159241;

const string PUNCT = ".,!?:\"();";


//Hash function for English words
//Polynomial accumulation
//the Horner's rule is used to compute the value
//See pag. 213 of course book
unsigned _hash(string s, int tableSize);

int main()
{
    

    string fileNr, temp, fileName;
    int initialTableSize;
    
    cout << "Select test file nr: ";
    cin >> fileNr;
    cout << "Enter initial table size: ";
    cin >> initialTableSize;
    cout << "Allow rehasing? (y/n)";
    cin >> temp;
    
    HashTable<string,int> freq_table(initialTableSize, _hash);
    if(temp == "n"){
        freq_table.disallowRehashing();
        cout << "rehashing disabled" << endl;;
    } 
    else{
        
        cout << "rehashing enabled" << endl;
    }
    
    fileName = "test_file" + fileNr + ".txt";
    ifstream file_in(fileName);
    ofstream file_out("out_"+fileName);

    if (!file_in || !file_out)
    {
        cout << "Could not open a file!!" << endl;

        return 0;
    }
    
    
    cout << "Reading file: '" << fileName << "'..." << endl << endl;
        
    string s;
    int _count = 0;
    
    //Read words and load them in the hash table
    while (file_in >> s)
    {
        //transform all upper-case letters to lower-case letters
        transform(s.begin(), s.end(), s.begin(), ::tolower);

        string s1;

        copy_if(s.begin(), s.end(), back_inserter(s1), [](char c)
        {
            return (PUNCT.find(c) == string::npos);
        });
        // cout << "HÄR!" << endl;
        //if s is not in the table then it is inserted
        freq_table[s1]++;

        _count++;
    }
    
    unsigned total = freq_table.get_total_visited_slots();

    cout << "\nNumber of words in the file = " << _count << endl;

    cout << "Number unique  words in the file = "
         << freq_table.get_number_OF_items() << endl;

    cout << "\nTable's load factor = "
         << fixed << setprecision(2) << freq_table.loadFactor() << endl;

    cout << "Number of calls to new Item() = "
         << freq_table.get_count_new_items() << endl << endl;

    cout << "\nNumber of slots visited = "
         << total << endl;

    cout << "Average Number of slots visited = "
         << fixed << setprecision(2) << (double)total / _count << endl;
    

    file_out << "Frequency table ..." << endl << endl;

    file_out << freq_table << endl;

    //close the file streams
    file_in.close();
    file_out.close();

    return 0;
}


//Hash function for English words
//Polynomial accumulation
//the Horner's rule is used to compute the value
//See pag. 213 of course book
unsigned _hash(string s, int tableSize)
{
    unsigned hashVal = 0;

    for(unsigned i = 0; i < s.length(); i++)
        hashVal = 37 * hashVal + s[i];

    hashVal %= tableSize;

    return hashVal;
}
