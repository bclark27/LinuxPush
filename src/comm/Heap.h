#include "Common.h"

///////////////
//  DEFINES  //
///////////////

#define HEAP_DEFAULT_SPACE    16

/////////////
//  TYPES  //
/////////////

typedef struct Heap
{
  void * heapSpace;
  CompareFunction compFunc;
  FreeDataFunction freeFunc;
  U4 dataLen;
  U4 elementSize;
  U4 heapSpaceBytes;
  bool passByVal;
  bool isMaxHeap;
} Heap;


/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

Heap * Heap_init(bool isMaxHeap, U4 dataLen, bool passByVal, CompareFunction compFunc, FreeDataFunction freeFunc);
void Heap_free(Heap * h);

void Heap_push(Heap * h, void * val);
void * Heap_peakTop(Heap * h);
void * Heap_popTop(Heap * h);
