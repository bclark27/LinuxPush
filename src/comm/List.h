#ifndef LIST_H_
#define LIST_H_

#include "Common.h"

/////////////
//  TYPES  //
/////////////

typedef struct Link
{
  void * data;
  struct Link * next;
  struct Link * prev;
} Link;

typedef struct List
{
  U4 len;
  struct Link * head;
  struct Link * tail;

  U4 dataLen;
  bool passByVal;
  CompareFunction compFunc;
  FreeDataFunction freeFunc;

} List;

typedef struct
{
  List * list;
  Link * curr;
} ListItr;

typedef List Queue;
typedef List Stack;

///////////////////////
//  STACK AND QUEUE  //
///////////////////////

#define Stack_init(dataLen, passByVal, freeFunc)  List_listInit(dataLen, passByVal, NULL, freeFunc)
#define Stack_free(s)                             List_destroyList(s)
#define Stack_push(s, data)                       List_push(s, data)
#define Stack_pop(s)                              List_pop(s)

#define Queue_init(dataLen, passByVal, freeFunc)  List_listInit(dataLen, passByVal, NULL, freeFunc)
#define Queue_free(q)                             List_destroyList(q)
#define Queue_queue(q, data)                      List_queue(q, data)
#define Queue_dequeue(q)                          List_dequeue(q)

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

List * List_listInit(U4 dataLen, bool passByVal, CompareFunction compFunc, FreeDataFunction freeFunc);
void List_listObjInit(List * list, U4 dataLen, bool passByVal, CompareFunction compFunc, FreeDataFunction freeFunc);
void List_iterateList(List * list, CallbackFunction callBack, void * args);
ListItr List_getItr(List * list);
void * List_getNextRef(ListItr * itr);
void * List_getNextVal(ListItr * itr);
void List_sortList(List * list);
void List_destroyList(List * list);
void List_clearList(List * list);
void List_queue(List * list, void * data);
void * List_dequeue(List * list);
void List_push(List * list, void * data);
void * List_pop(List * list);
void * List_getVal(List * list, U4 index);
void * List_getRef(List * list, U4 index);
void * List_removeItem(List * list, U4 index);
void List_deleteItem(List * list, U4 index);
void List_destroyLink(List * list, Link * link);
void List_destroyLinkAndData(List * list, Link * link);

#endif
