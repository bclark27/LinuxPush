#ifndef HASH_SET_H_
#define HASH_SET_H_

#include "Common.h"
#include "HashTable.h"

/////////////
//  TYPES  //
/////////////

typedef struct
{
  HashTable * ht;
  FreeDataFunction freeDataFunc;
  HashFunction keyHashFunc;
  U4 keyLen;
} HashSet;

/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

HashSet * HashSet_init(U4 keyLen, bool passByRef, FreeDataFunction freeDataFunc, HashFunction keyHashFunc);
void HashSet_free(HashSet * hs);

void HashSet_iterate(HashSet * hs, CallbackFunction callBack, void * args);
bool HashSet_insert(HashSet * hs, void * key);
void HashSet_remove(HashSet * hs, void * key);
bool HashSet_keyIn(HashSet * hs, void * key);

#endif
