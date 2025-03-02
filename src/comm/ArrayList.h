#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include "Common.h"

///////////////
//  DEFINES  //
///////////////

#define DEFAULT_LIST_LEN 100

/////////////
//  TYPES  //
/////////////

typedef struct ArrayList
{
  unsigned char * data;
  unsigned int listLen;
  unsigned int elementSize;
  unsigned int elementCount;
} ArrayList;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

ArrayList * ArrayList_init(unsigned int elementSize, unsigned int initSize);
void ArrayList_freeList(ArrayList * al);

void ArrayList_append(ArrayList * al, void * data);
bool ArrayList_insert(ArrayList * al, unsigned int index, void * data);
void * ArrayList_get(ArrayList * al, unsigned int index);
void ArrayList_delete(ArrayList * al, unsigned int index);
void ArrayList_eraseList(ArrayList * al);

#endif
