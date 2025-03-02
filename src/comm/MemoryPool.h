#ifndef MEMORY_POOL_H_
#define MEMORY_POOL_H_


#include "Common.h"

#include "EntityProperty.h"
#include "List.h"

///////////////
//  DEFINES  //
///////////////

/*
 * Return Codes
 */
#define MP_SUCCESS 0
#define MP_ERR_MEMPOOL_NULL -1
#define MP_ERR_ITEM_NULL -2
#define MP_ERR_ITEM_NOT_IN_MEMPOOL -3

/////////////
//  TYPES  //
/////////////

typedef struct MemoryPool
{
    U4 objSize;
    void* firstSection;
} MemoryPool;

typedef struct MemoryPoolStats
{
    U4 objSize;
    U4 maxObjPerSection;
    U4 sectionCount;
    U4 emptySectionCount;
    U4 fullSectionCount;
    U4 totalObjInUseCount;
    U4 totalObjAvailableCount;
    
    double totalMemEfficiency;

} MemoryPoolStats;

// arg0: data start, arg1: percistant state
typedef bool (*MemoryPool_IterCallback)(void *, void *);
// arg0: data start, arg1: percistant state, arg2: count, arg3: dataSize
typedef bool (*MemoryPool_IterBatchCallback)(void *, void *, U4, U4);

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

// init the pool. elements in the pool will all be of size dataSize
MemoryPool * MemoryPool_init(U4 dataSize);

// frees the pool
void MemoryPool_free(MemoryPool * mp);

// allocates memory for a new item. returns success or error code
int MemoryPool_AddItem(MemoryPool* mp);
int MemoryPool_AddItemInitialData(MemoryPool* mp, void* initData);

void* MemoryPool_GetCurrentItemPtr(MemoryPool* mp, U4 idx);

// using this pointer to the item in the pool, remove it. if it doesnt exist then do nothing or return error code
int MemoryPool_RemoveItem_Ptr(MemoryPool* mp, void* item);
int MemoryPool_RemoveItem_Idx(MemoryPool* mp, U4 idx);

// iter over all the elements
void MemoryPool_Iter(MemoryPool* mp, MemoryPool_IterCallback process, void* state);
void MemoryPool_IterBatch(MemoryPool* mp, MemoryPool_IterBatchCallback process, void* state);

void MemoryPool_Defragment(MemoryPool* mp);

void MemoryPool_GetStats(MemoryPool* mp, MemoryPoolStats* stats);

#endif