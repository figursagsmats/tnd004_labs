/*********************************************
* file:	~\tnd004\lab\lab4a\digraph.cpp       *
* remark: mplementation of directed graphs   *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

#include "digraph.h"
#include "queue.h"

const int INFINITY = 9999;
void printPartRec(int t, int* path);
// -- CONSTRUCTORS

Digraph::Digraph(int n)
{
    assert(n >= 1);
    array = new List[n + 1];
    dist  = new int [n + 1];
    path  = new int [n + 1];
    done  = new bool[n + 1];
    size  = n;
}

// -- DESTRUCTOR

Digraph::~Digraph()
{
    delete[] array;
    delete[] dist;
    delete[] path;
    delete[] done;
}

// -- MEMBER FUNCTIONS

// insert directed edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Digraph::insertEdge(int u, int v, int w)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].insert(v, w);
}

// remove directed edge (u, v)
void Digraph::removeEdge(int u, int v)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].remove(v);
}

// unweighted single source shortest paths
void Digraph::uwsssp(int s)
{
    if (s < 1 || s > size)
    {
         cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
         return;
    }
    Queue<int> Q;

    for(int i = 0; i < size + 1; i++)
    {
        dist[i] = INFINITY;
        path[i] = -1;
    }
    dist[s] = 0;
    Q.enqueue(s);

    while(!Q.isEmpty())
    {
        int v = Q.getFront();
        Q.dequeue();

        Node *n = array[v].getFirst();
        while(n != nullptr)
        {
            if(dist[n->vertex] == INFINITY)
            {
                dist[n->vertex] = dist[v] + 1;
                path[n->vertex] = v;
                Q.enqueue(n->vertex);
            }
            n = array[v].getNext();
        }
    }
}

// positive weighted single source shortest pats
void Digraph::pwsssp(int s)
{
    if (s < 1 || s > size)
    {
         cout << "\nERROR: expected source s in range 1.." << size << " !" << endl;
         return;
    }

    for(int i = 0; i < size + 1; i++)
    {
        dist[i] = INFINITY;
        path[i] = -1;
        done[i] = false;
    }
    dist[s] = 0;
    done[s] = true;

    int v = s; //

    while(1)
    {
        Node *n = array[v].getFirst();
        while(n != nullptr)
        {
            bool isDone = done[n->vertex];
            bool isBetterPath = dist[n->vertex] > dist[v] + n->weight;
            if( !isDone&& isBetterPath )
            {
                dist[n->vertex] = dist[v] + n->weight;
                path[n->vertex] = v;
            }
            n = array[v].getNext();
        }

        //find nearest closest
        int currentMin = INFINITY;
        //Iterate over all verticies
        for(int i = 1; i < size + 1; i++)
        {
            if(!done[i] && dist[i] < currentMin)
            {
                v = i;
                currentMin = dist[i];
            }
        }
        if(currentMin == INFINITY)
            break;
        done[v] = true;
    }
}

// print graph
void Digraph::printGraph() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "vertex  adjacency list                  " << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " : ";
        array[v].print();
    }

    cout << "------------------------------------------------------------------" << endl;
}

// print shortest path tree for s
void Digraph::printTree() const
{
    cout << "----------------------" << endl;
    cout << "vertex    dist    path" << endl;
    cout << "----------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " :" << setw(8) << dist[v] << setw(8) << path[v] << endl;
    }

    cout << "----------------------" << endl;
}

//rekursion sv�rt, kunde inte skriva ut snyggt utan denna hj�lpfunktion
void printPartRec(int t, int* path)
{
    if(path[t] != -1)
        printPartRec(path[t], path);
    cout << " " << t;
}

// print shortest path from s to t
void Digraph::printPath(int t) const
{
    if (t < 1 || t > size)
    {
         cout << "\nERROR: expected target t in range 1.." << size << " !" << endl;
         return;
    }

    printPartRec(t, path);
    cout << " (" << dist[t] << ")" << endl;
}
