#include"linked_list_structs.h"

#ifndef LINKED_LIST_FUNCS
#define LINKED_LIST_FUNCS
linked_list* create_list();
void append_to_front(linked_list* ll, void* item);
void append_to_end(linked_list* ll, void* item);
void delete_item(linked_list* ll, void* item);
void delete_link(linked_list* ll, linked_list_link* link);
void* pop(linked_list* ll);
void push(linked_list* ll, void* item);
void queue(linked_list* ll, void* item);
linked_list_link* get_next_link(linked_list_link* this_link);
void free_list(linked_list* ll);
#endif
