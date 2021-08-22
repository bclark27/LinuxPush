#ifndef LINKED_LIST
#define LINKED_LIST
typedef struct linked_list_link{
  struct linked_list_link* next;
  struct linked_list_link* prev;
  void* data;
}linked_list_link;

typedef struct linked_list{
  struct linked_list_link* head;
  struct linked_list_link* tail;
}linked_list;
#endif
