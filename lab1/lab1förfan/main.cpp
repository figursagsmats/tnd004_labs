/*
  Author: Aida Nordman
  Course: TND004, Lab 1
  Description: test program
*/

#include <iostream>
#include <iomanip>
#include <string>

#include "set.h" //file with the template Set class definition

using namespace std;


//Do not modify
int main()
{
    /*****************************************************
    * TEST PHASE 0                                       *
    * Default constructor, conversion constructor, and   *
    * operator<<                                         *
    ******************************************************/
    cout << "TEST PHASE 0: default and conversion constructor\n\n";

    Set<int> S1;
    Set<int> S2(-4);

    cout << "S1 = " << S1 << endl;
    cout << "S2 = " << S2 << endl;

    /*****************************************************
    * TEST PHASE 1                                       *
    * Constructor: create a Set from an array            *
    ******************************************************/
    cout << "\nTEST PHASE 1: constructor from an array\n\n";

    int A1[] = { 1, 3, 5 };
    int A2[] = { 2, 3, 4 };

    Set<int> S3(A1, 3);
    Set<int> S4(A2, 3);

    cout << "S3 = " << S3 << endl;
    cout << "S4 = " << S4 << endl;

    /*****************************************************
    * TEST PHASE 2                                       *
    * Copy constructor                                   *
    ******************************************************/
    cout << "\nTEST PHASE 2: copy constructor\n\n";

    Set<int> S5(S3);

    cout << "S5 = " << S5 << endl;

    /*****************************************************
    * TEST PHASE 3                                       *
    * Assignment operator: operator=                     *
    ******************************************************/
    cout << "\nTEST PHASE 3: operator=\n\n";

    S1 = S2 = S5;

    cout << "S1 = " << S1 << endl;
    cout << "S2 = " << S2 << endl;

    /*****************************************************
    * TEST PHASE 4                                       *
    * Move constructor                                   *
    ******************************************************/
    cout << "\nTEST PHASE 4: move constructor\n\n";

    Set<int> *ptr_S = new Set<int>(S4);

    Set<int> S6(std::move(*ptr_S));

    cout << "S6 = " << S6 << endl;

    delete ptr_S;


    /*****************************************************
    * TEST PHASE 5                                       *
    * move assignment operator                           *
    ******************************************************/
    cout << "\nTEST PHASE 5: move assignment operator\n\n";

    ptr_S = new Set<int>(S2);

    S6 = std::move(*ptr_S);

    cout << "S6 = " << S6 << endl;

    delete ptr_S;


    /*****************************************************
    * TEST PHASE 6                                       *
    * is_member                                          *
    ******************************************************/
    cout << "\nTEST PHASE 6: is_member\n\n";

    for (int i = 1; i <= 5; i++)
    {
        cout << "S1.is_member(" << i << ") ? "
             << (S1.is_member(i) ? 'T' : 'F') << endl;
    }
    cout << "\nS1.is_member(99999) ? "
         << (S1.is_member(99999) ? 'T' : 'F') << endl;

    /*****************************************************
    * TEST PHASE 7                                       *
    * cardinality, make_clear                            *
    ******************************************************/
    cout << "\nTEST PHASE 7: cardinality\n\n";

    cout << "S1.cardinality() = "
         << S1.cardinality() << endl;

    cout << "S1 becomes the empty set ..." << endl;

    S1.make_empty();

    cout << "S1.cardinality() = "
         << S1.cardinality() << endl;

    /*****************************************************
    * TEST PHASE 8                                       *
    * Overloaded operators: operator+=, operator*=       *
    *                   and operator-=                   *
    ******************************************************/
    cout << "\nTEST PHASE 8: operator+=, operator*=, operator-=\n\n";

    S6 += S4;
    S4 *= S4;
    S1 -= S1;

    cout << "S6 = " << S6 << endl;
    cout << "S4 = " << S4 << endl;
    cout << "S1 = " << S1 << endl;


    /*****************************************************
    * TEST PHASE 9                                       *
    * Overloaded operators: union, intersection, and     *
    * and difference                                     *
    ******************************************************/
    cout << "\nTEST PHASE 9: union, intersection, and difference\n\n";

    S1 = (S3 + S4);
    S2 = S3 * S4;
    S5 = S3 - S4;

    Set<int> S7(S3);
    S6 = std::move(S7) + S5;

    cout << "S1 = " << S1 << endl;
    cout << "S2 = " << S2 << endl;
    cout << "S3 = " << S3 << endl;
    cout << "S4 = " << S4 << endl;
    cout << "S5 = " << S5 << endl;
    cout << "S6 = " << S6 << endl;

    /*****************************************************
    * TEST PHASE 10                                       *
    * Overloaded operators: equality, subset, and        *
    * strict subset                                      *
    ******************************************************/
    cout << "\nTEST PHASE 10: equality, subset, strict subset\n\n";

    cout << "S4 == S1 ? " << (S4 == S1 ? 'T' : 'F') << endl;
    cout << "S4!== S1 ? " << (S4 != S1 ? 'T' : 'F') << endl;
    cout << "S4 <= S1 ? " << (S4 <= S1 ? 'T' : 'F') << endl;
    cout << "S4 <  S1 ? " << (S4 <  S1 ? 'T' : 'F') << endl;

    /*****************************************************
    * TEST PHASE 11                                       *
    * Overloaded operators: mixed-mode arithmetic        *
    ******************************************************/
    cout << "\nTEST PHASE 11: mixed-mode arithmetic\n\n";

    //Note: conversion constructor is called
    S2 = 4 + S3 - 5 - (S3+S4) - 99999;
    S5 = S4 - 2 + S2 + 24;

    cout << "S2 = " << S2 << endl;
    cout << "S5 = " << S5 << endl;

    /*****************************************************
    * TEST PHASE 12                                       *
    * Create a set of strings                            *
    ******************************************************/
    cout << "\nTEST PHASE 12: Create a set of strings\n\n";

    string AA[] = {"000", "001", "010", "011", "100"};
    Set<string> words(AA, 5);

    words = string("101") + words - string("000");

    cout << "The binary strings sorted: "
         << words << endl;

    words.make_empty();

    cout << "The binary strings: " << words << endl;

	cout << "Ending ...." << endl;

    return 0;
}
