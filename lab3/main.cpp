#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>


#include "BinarySearchTree.h"

using namespace std;

template<typename Comparable>
class Row
{
private:
    Comparable element;
    int counter;
public:
    Row() = default;
    Row(Comparable c): element{c}, counter{int{1}} {}
    Row& operator++(){
        counter += 1;
        return *this;
    }
    friend bool operator<(const Row& l, const Row& r){
        return l.element < r.element;
        return l.counter < r.counter;
    }
    friend bool operator>(const Row& l, const Row& r){
        return l.element > r.element;
        return l.counter > r.counter;
    }
    friend ostream& operator<<(ostream& os, Row& r){

        os << setw(18) << r.element << "  :  " << r.counter;
        return os;
    }
     bool operator!=(Row &r)
    {
        return this->element != r.element;
    }
};

 //Test program 3: BiIterator
int main( )
{
   BinarySearchTree<Row<string>> T;
    ifstream file("words.txt");

    if (!file)
    {
        cout << "couldn't open file words.txt" << endl;
        return 1;
    }

    vector<string> V1 = { istream_iterator<string>{file}, istream_iterator<string>{} };
    file.close();

    for(auto j: V1) //special code in insert is used for this ex4. should be uncommented else
        T.insert( j );



    for(auto it = T.begin(); it != T.end(); it++)
        cout << it->element << endl;

    return 0;
}
