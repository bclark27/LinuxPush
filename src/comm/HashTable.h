#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "Common.h"
#include "List.h"
#include "FastHash.h"

///////////////
//  DEFINES  //
///////////////

#define DEFAULT_HASH_TABLE_LENGTH   421

/////////////
//  TYPES  //
/////////////

typedef struct
{
  void * key;
  void * val;
} HashKVPair;

typedef struct
{
  void * key;
  FreeDataFunction freeKeyFunc;
  HashFunction keyHashFunc;

  void * val;
  FreeDataFunction freeValFunc;

  U4 keyLen;
  U4 valLen;

  bool valPassByRef;
  bool keyPassByRef;
} HashTableLink;

typedef struct
{
  List * table;
  U4 size;
  U4 elementCount;
  bool valPassByRef;
  bool keyPassByRef;
} HashTable;

/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

HashTable * HashTable_init(bool keyPassByRef, bool valPassByRef);
void HashTable_free(HashTable * ht);

bool HashTable_insert(HashTable * ht, void * key, U4 keyLen, FreeDataFunction freeKeyFunc, void * val, U4 valLen, FreeDataFunction freeValFunc, HashFunction keyHashFunc);
void * HashTable_getRef(HashTable * ht, void * key, U4 keyLen, U4 * valLen, HashFunction keyHashFunc);
void * HashTable_getVal(HashTable * ht, void * key, U4 keyLen, U4 * valLen, HashFunction keyHashFunc);
void HashTable_remove(HashTable * ht, void * key, U4 keyLen, HashFunction keyHashFunc);
bool HashTable_keyIn(HashTable * ht, void * key, U4 keyLen, HashFunction keyHashFunc);
bool HashTable_valIn(HashTable * ht, void * key, U4 keyLen, HashFunction keyHashFunc);
void HashTable_iterateTableVals(HashTable * ht, CallbackFunction callBack, void * args);
void HashTable_iterateTableKeys(HashTable * ht, CallbackFunction callBack, void * args);
void HashTable_iterateTableKV(HashTable * ht, CallbackFunction callBack, void * args);

#endif
