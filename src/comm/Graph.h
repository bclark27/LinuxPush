#ifndef GRAPH_H_
#define GRAPH_H_

#include "Common.h"
#include "TypedHashTable.h"

/////////////
//  TYPES  //
/////////////

typedef struct
{
  void * a;
  void * b;
} GraphEdge;

typedef struct
{
  TypedHashTable * ht;
  HashFunction nodeHashFunc;
  FreeDataFunction freeNodeFunction;
  U4 valLen;
  bool directed;
} Graph;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

Graph * Graph_init(U4 valLen, bool directed, FreeDataFunction freeNodeFunction, HashFunction nodeHashFunc);
void Graph_free(Graph * g);

bool Graph_addEdge(Graph * g, void * a, void * b);
bool Graph_addNode(Graph * g, void * a);
void Graph_depthFirst(Graph * g, void * startNode, CallbackFunction callback, void * args);
void Graph_breadthFirst(Graph * g, void * startNode, CallbackFunction callback, void * args);
void Graph_iterateEdges(Graph * g, CallbackFunction callback, void * args);

#endif
