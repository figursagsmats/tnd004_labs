/*********************************************
* file:	~\tnd004\lab\lab4b\graph.h           *
* remark:implementation of undirected graphs *
* file for students                          *
**********************************************/


#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

#include "graph.h"
#include "edge.h"
#include "heap.h"
#include "dsets.h"

const int INFINITY = 9999;

// -- CONSTRUCTORS

Graph::Graph(int n)
{
    assert(n >= 1);
    array = new List[n + 1];
    size  = n;
}

// -- DESTRUCTOR

Graph::~Graph()
{
    delete[] array;
}

// -- MEMBER FUNCTIONS

// insert undirected edge (u, v) with weight w
// update weight w if edge (u, v) is present
void Graph::insertEdge(int u, int v, int w)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].insert(v, w);
    array[v].insert(u, w);
}

// remove undirected edge (u, v)
void Graph::removeEdge(int u, int v)
{
    assert(u >= 1 && u <= size);
    assert(v >= 1 && v <= size);
    array[u].remove(v);
    array[v].remove(u);
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const
{
    int *dist = new int[size + 1];
    int *path = new int[size + 1];
    bool *done = new bool[size + 1];

    int start = 1;
    int totalDist = 0;

    for(int i = 1; i <= size; i++)
    {
        dist[i] = INFINITY;
        path[i] = -1;
        done[i] = false;
    }

    dist[start] = 0;
    done[start] = true;
    int v = start;

    while(1)
    {
        Node *n = array[v].getFirst();
        while(n != nullptr)
        {
            if( !done[n->vertex] && dist[n->vertex] > n->weight )
            {
                dist[n->vertex] = n->weight;
                path[n->vertex] = v;
            }
            n = n->next;
        }

        //find nearest closest
        int currentMin = INFINITY;
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
        cout << "( " << path[v] << ", " << v << ", " <<dist[v] << ")" << endl;
        totalDist += dist[v];
    }
    cout << endl << "total weight: " << totalDist << endl;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const
{
    int v, counter, totalDist = 0;

    Heap<Edge> H;
    H.makeEmpty();

    DSets D(size);
    D.init(); //init to -1, each vertex is a seperate tree

    //build the heap with all edges
    for(v = 1; v <= size; v++)
    {
        Node *n = array[v].getFirst();
        while(n != nullptr)
        {
            //stupid if/else to allways set edge to a nice looking smaller -> larger
            //as in the sugested output. dont know how to make that happen otherwise
            if(v < n->vertex)
                H.insert(Edge(v, n->vertex, n->weight));
            else
               H.insert(Edge(n->vertex, v, n->weight));
            n = n->next;
        }
    }

    counter = 0;
    Edge tmp;

    while(counter < size -1)
    {
        tmp = H.deleteMin(); //find edge with lowest cost
        if(D.find(tmp.head) != D.find(tmp.tail)) //belong to same tree?
        {
            cout << tmp << endl;
            totalDist += tmp.weight;
            D.join(D.find(tmp.head), D.find(tmp.tail)); //merge two trees
            counter++;
        }
    }
    cout << endl << "total weight: " << totalDist << endl;
}

// print graph
void Graph::printGraph() const
{
    cout << "------------------------------------------------------------------" << endl;
    cout << "vertex  adjacency list                                            " << endl;
    cout << "------------------------------------------------------------------" << endl;

    for (int v = 1; v <= size; v++)
    {
        cout << setw(4) << v << " : ";
        array[v].print();
    }

    cout << "------------------------------------------------------------------" << endl;
}
