#ifndef LIST_H_
#define LIST_H_

/////////////
//  TYPES  //
/////////////

typedef void (* FreeDataFunction)(void *);
typedef char (* CompareFunction)(void *, void *);

typedef struct Link
{
  void * data;
  struct Link * next;
  struct Link * prev;
} Link;

typedef struct List
{
  unsigned int len;
  struct Link * head;
  struct Link * tail;
} List;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

List * listInit();
unsigned int listLen();

void * findByProperty(List * list, void * property, CompareFunction compFunc);

void sortList(List * list, CompareFunction compFunc);

void destroyList(List * list, FreeDataFunction freeFunc);
void destroyListDynamic(List * list);

void clearList(List * list, FreeDataFunction freeFunc);
void clearListDynamic(List * list);

char queue(List * list, void * data);
void * dequeue(List * list);

void deleteItem(List * list, void * data, FreeDataFunction freeFunc);
void deleteItemDynamic(List * list, void * data);

char inList(List * list, void * data);

void _destroyLink(List * list, Link * link, FreeDataFunction freeFunc);
void _destroyLinkDynamic(List * list, Link * link);

#endif
